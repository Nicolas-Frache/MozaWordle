#include <string.h>
#include <stdio.h>
#include <tgmath.h>
#include "solver.h"
#include "attempts_tools.h"

#define COLOR_OFF   "\e[m"
#define COLOR_BOLD_BLUE  "\e[1;34m"

list_attempts_t *previous_attempts;

void init_previous_attempts(int word_size) { previous_attempts = create_list_attempts(word_size); };

list_attempts_t *get_previous_attempt() { return previous_attempts; }

char *compute_next_best_attempt() {
    word_t *current_possible_head = get_current_possible()->head;

    // Si on a un nombre de mots possibles <= 3 on essaie directement le premier qui vient,
    // En effet, chercher dans l'ensemble du dictionnaire le mot qui éliminera en moyenne le plus de
    // réponse possible n'est pas intéressant en dessous de 4 car on aura dans tous les cas 1 ou 2 essaie pour avoir la
    // solution, donc autant avoir une chance d'y arriver du premier coup
    if (get_current_possible()->nb_words <= 3) {
        printf("\n\n● Mot avec la plus grande entropie:   "COLOR_BOLD_BLUE"%s\n"COLOR_OFF,
               current_possible_head->word);
        return current_possible_head->word;
    }

    // Pour tous les mots du dictionnaire on calcule une espérance
    words_list_t *dict = get_dictionary();

    // On déclare un maximum de truc en dehors des sous-boucles plus profondes pour gagner en temps d'exécution
    word_t *mot_candidat = dict->head;
    word_t *word_with_max_entropy = NULL;
    word_t *mot_possible;
    constraints_t *new_constraints;
    double max_entropy = 0;
    int *patterns = generate_patterns(dict->words_size);
    int nb_possible_answers = get_current_possible()->nb_words;

    double matching_word_pattern_nb, entropy_of_word, prob_x, info_x;
    list_attempts_t *new_list_attempts = create_list_attempts(dict->words_size);


    // On fait le tour des mots du dictionnaire pour trouver celui qui a la meilleure entropie en moyenne
    for (int i = 0; i < dict->nb_words; i++) {
        entropy_of_word = 0;

        // Récupération de tous les patterns possibles :
        // Pour tous les patterns on calcul l'entropie du pattern
        for (int j = 0; j < pow(3, dict->words_size); j++) {
            //list_attempts_t *new_list_attempts = create_list_attempts(dict->words_size);
            //append_attempt(new_list_attempts, mot_candidat->word, &patterns[j * dict->words_size]);
            free(new_list_attempts->head);
            new_list_attempts->head = create_attempt(mot_candidat->word, &patterns[j * dict->words_size]);

            new_constraints = compute_constraints_from_attempts(new_list_attempts);
            mot_possible = current_possible_head;

            matching_word_pattern_nb = 0;
            // Pour tous les mots encores possibles avec les indices precedents :
            while (mot_possible != NULL) {
                // Il ne faut surtout ne rien mettre d'inutile ici, chaque instruction dans cette boucle est
                // exécutée taille_dict * 3^n * nombre_mot_possible fois.
                // Soit par exemple pour 5 lettres et la première tentative : 2.7 milliards de fois sans optimisation particulière
                if (is_matching_word_constraints(mot_possible->word, new_constraints)) {
                    matching_word_pattern_nb++;
                }
                mot_possible = mot_possible->next;
            }
            if (matching_word_pattern_nb != 0) {


                // Probabilité que ce soit bien ce pattern qui tombe en jouant ce mot
                prob_x = matching_word_pattern_nb / nb_possible_answers;
                // Nombre de bit d'information que donnerait ce pattern s'il tombait
                // Un bit correspond à une division par deux du nombre de mots possibles
                info_x = log2(1 / prob_x);
                // Calcul de l'entropie global du mot en sommant les produits de la proba de chaque pattern avec son entropie
                entropy_of_word += prob_x * info_x;
            }
            destroy_constraints(new_constraints);
        }

        // Si le mot qu'on vient de tester et celui avec la plus grande entropie, on le conserve
        if (entropy_of_word > max_entropy) {
            max_entropy = entropy_of_word;
            word_with_max_entropy = mot_candidat;
        }

        //printf("\n> %d/%d,      %s          | %f bits", i, dict->nb_words, mot_candidat->word, entropy_of_word);
        mot_candidat = mot_candidat->next;
    }
    free(patterns);
    destroy_list_attempts(new_list_attempts);
    printf("\n\n● Mot avec la plus grande entropie:   "COLOR_BOLD_BLUE"%s\n"COLOR_OFF,
           word_with_max_entropy->word
    );

    return word_with_max_entropy->word;
}


int compute_constraints_improvement(constraints_t *old, constraints_t *new) {
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
