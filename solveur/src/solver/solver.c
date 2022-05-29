#include <math.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <tgmath.h>
#include "solver.h"
#include "attempts_tools.h"

list_attempts_t *previous_attempts;
list_attempts_t* get_previous_attempt() { return previous_attempts;}

char *compute_next_best_attempt() {
    // Pour tous les mots du dictionnaire on calcul un esperance
    words_list_t *dict = get_dictionary();
    // Le tableau a une taille déterminée durant l'exécution, il faut donc l'initialiser manuellement

    word_t *mot_candidat = dict->head;
    double max_entropy = 0;
    word_t *word_with_max_entropy = NULL;
    int *patterns = generate_patterns(dict->words_size);


    for (int i = 0; i < dict->nb_words; i++) {
        double entropy_of_word = 0;
        // Récupération de tous les patterns possibles :

        // Pour tous les patterns on calcul l'entropie partielle, on les additionne au fur est à mesure pour avoir l'entropie du mot candidat
        for (int j = 0; j < pow(3, dict->words_size); j++) {
            list_attempts_t *new_list_attempts = create_list_attempts(dict->words_size);
            char *mot_candidat_cpy = malloc(strlen(mot_candidat->word) + 1 * sizeof(char));
            strcpy(mot_candidat_cpy, mot_candidat->word);
            int *pattern_cpy = malloc(sizeof(int) * dict->words_size);
            memcpy(pattern_cpy, &patterns[j * dict->words_size], 1);
            append_attempt(new_list_attempts, mot_candidat_cpy, pattern_cpy);

            constraints_t *new_constraints = compute_constraints_from_attempts(new_list_attempts);



            word_t *mot_possible = get_current_possible()->head;
            double matching_word_pattern_nb = 0;
            // Pour tous les mots encores possibles avec les indices precedents :
            while (mot_possible != NULL) {
                bool match = is_matching_word_constraints(mot_possible->word, new_constraints);
                if (match) {
                    matching_word_pattern_nb++;
                }
                mot_possible = mot_possible->next;
            }
            double prob_x = matching_word_pattern_nb / get_current_possible()->nb_words;
            double info_x = log2(1 / prob_x);
            // Calcul de l'entropie global du mot en additionnant l'entropie partielle de tous les patterns
            entropy_of_word += prob_x * info_x;

            free(mot_candidat_cpy);
            free(pattern_cpy);
            destroy_constraints(new_constraints);
            destroy_list_attempts(new_list_attempts);
        }

        // Si le mot qu'on vient de tester et celui avec la plus grande entropie, on le conserve
        if(entropy_of_word > max_entropy){
            max_entropy = entropy_of_word;
            word_with_max_entropy = mot_candidat;
        }

        printf("\n> %d/%d,      %s          | %f bits", i, dict->nb_words, mot_candidat->word, entropy_of_word);
        mot_candidat = mot_candidat->next;
    }
    free(patterns);

    printf("\n\n> Mot avec la plus grande entropie: %s, entropie: %f bits\n", word_with_max_entropy->word, max_entropy);
    return word_with_max_entropy->word;
}


int compute_constraints_improvement(constraints_t *old, constraints_t *new) {
    //Non testé
    int data = 0;

    word_constraint_t *old_w = old->word_constraint;
    word_constraint_t *new_w = new->word_constraint;
    for (int i = 0; i < NB_LETTERS; i++) {
        //Un bit d'info si on apprend que le nombre de lettre est exact
        if (old_w->is_exact_nb_occurrences_letters[i] != new_w->is_exact_nb_occurrences_letters[i]) { data++; }
        //Un bit d'info par occurence minimum que l'on apprendq
        if (old_w->min_nb_occurrences_letters[i] < new_w->min_nb_occurrences_letters[i]) {
            data += new_w->min_nb_occurrences_letters[i] - old_w->min_nb_occurrences_letters[i];
        }
    }
    int n = old->word_size;
    for (int i = 0; i < n; i++) {
        emplacement_constraints_t old_empl = old->emplacement_constraints[i];
        emplacement_constraints_t new_empl = new->emplacement_constraints[i];
        if (old_empl.has_a_mandatory_letter !=
            new_empl.has_a_mandatory_letter) { data++; } // Un bit d'info si on apprend qu'une lettre est obligatoir
        for (int j = 0; j < NB_LETTERS; j++) {
            if (old_empl.forbidden_letters[j] !=
                new_empl.forbidden_letters[j]) { data++; } // Si on apprend qu'une lettre est interdite, +1 info
        }
    }
    return data;
}

int evaluate_score_with_specific_combination(char *candidate_word, char *matching_word, constraints_t *constraints) {
    //Non testé
    constraints_t *new_constraint = copy_constraints(constraints);
    attempt_t *attempt = create_attempt_and_result(candidate_word, matching_word);
    update_constraints_with_attempts(new_constraint,
                                     attempt);  // On regarde les nouvelles contraintes si on propose candidate_word et que le mot valide est matching word

    return compute_constraints_improvement(constraints, new_constraint); // On calcul les nouvelles informations
}

int *generate_patterns(int length) {

    int NB_PATTERN = pow(3, length);
    int *table = calloc(1, sizeof(int) * NB_PATTERN * length);

    char str[length + 1];
    for (int i = 0; i < NB_PATTERN; i++) {
        sprintf(str, "%d", convert(i, 3));

        //Remplissage de la table
        for (int j = strlen(str) - 1; j >= 0; j--) {
            table[i * length + strlen(str) - j - 1] = str[j] - 48;
        }
    }

    return table;
}

int convert(int number, int base) {
    if (number == 0 || base == 10)
        return number;
    return (number % base) + 10 * convert(number / base, base);
}
