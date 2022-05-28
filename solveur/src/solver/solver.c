#include <math.h>
#include <string.h>
#include <stdio.h>
#include "solver.h"
#include "attempts_tools.h"


int compute_constraints_improvement(constraints_t* old, constraints_t* new)
{
    //Non testé
    int data = 0;

    word_constraint_t* old_w = old->word_constraint;
    word_constraint_t* new_w = new->word_constraint;
    for (int i = 0; i < NB_LETTERS; i++)
    {
        //Un bit d'info si on apprend que le nombre de lettre est exact
        if (old_w->is_exact_nb_occurrences_letters[i] != new_w->is_exact_nb_occurrences_letters[i]) { data++;}
        //Un bit d'info par occurence minimum que l'on apprendq
        if (old_w->min_nb_occurrences_letters[i] < new_w->min_nb_occurrences_letters[i]) { data+= new_w->min_nb_occurrences_letters[i] - old_w->min_nb_occurrences_letters[i];}    
    }
    int n = old->word_size;
    for (int i = 0; i < n; i++)
    {
        emplacement_constraints_t old_empl = old->emplacement_constraints[i];
        emplacement_constraints_t new_empl = new->emplacement_constraints[i];
        if (old_empl.has_a_mandatory_letter != new_empl.has_a_mandatory_letter) { data++;} // Un bit d'info si on apprend qu'une lettre est obligatoir
        for (int j = 0; j < NB_LETTERS; j++)
        {
            if (old_empl.forbidden_letters[j] != new_empl.forbidden_letters[j]) { data++;} // Si on apprend qu'une lettre est interdite, +1 info
        }
    }
    return data;
}

int evaluate_score_with_specific_combination(char *candidate_word, char *matching_word, constraints_t* constraints)
{
    //Non testé
    constraints_t* new_constraint = copy_constraints(constraints);
    attempt_t* attempt = create_attempt_and_result(candidate_word, matching_word); 
    update_constraints_with_attempts(new_constraint,attempt);  // On regarde les nouvelles contraintes si on propose candidate_word et que le mot valide est matching word

    return compute_constraints_improvement(constraints, new_constraint); // On calcul les nouvelles informations
}

int *generate_patterns(int length) {

    int NB_PATTERN = pow(3, length);
    int *table = calloc(1, sizeof(int) * NB_PATTERN * length);

    char str[length+1];
    for (int i=0;i<NB_PATTERN;i++) {
        sprintf(str, "%d", convert(i, 3));

        //Remplissage de la table
        for (int j = strlen(str)-1;j>=0;j--) {
            table[i * length + strlen(str)-j-1] = str[j]-48;
        }
    }

    return table;
}

int convert(int number,int base) {
    if(number == 0 || base==10)
        return number;
    return (number % base) + 10*convert(number / base, base);
}
