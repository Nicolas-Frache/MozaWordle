#include <stdio.h>
#include <string.h>
#include "dict.h"

#define UNUSED(x) (void)(x)

void import_dict(int word_size) {
    UNUSED(word_size);
    // TODO
}

struct words_list_t *get_all_matching_words(list_attempts_t *list_tries, struct words_list_t *list_words) {
    UNUSED(list_tries);
    UNUSED(list_words);
    // TODO
    return NULL;
}

bool is_matching_word_specific_attempts(char *word, list_attempts_t *attempts) {
    UNUSED(word);
    UNUSED(attempts);
    if (is_empty_list_attempts(attempts))
        return true;

    attempt_t *current = attempts->head;
    bool result = is_matching_word_one_specific_attempt_machine_a_gaz_edition(word, current);
    while (current->next != NULL && result) {
        current = current->next;
        result &= is_matching_word_one_specific_attempt_machine_a_gaz_edition(word, current);
    }
    return result;
}

bool is_matching_word_one_specific_attempt_machine_a_gaz_edition(char *word, attempt_t *attempt) {
    int len = (int) strlen(word);
    for (int i = 0; i < len; i++) {

        // On vérifie que toute les lettres dont connait la place directement (code 2),
        // sont bien au bon endroit
        if (attempt->results[i] == 2 && word[i] != attempt->word[i]) {
            return false;
        }

        // On vérifie que toutes les lettres dont on sait qu'elles sont à la mauvaise place (code 1),
        // sont bien présentes en bon nombre dans le mot.
        // Par exemple : Si on a le mot "aaa...." et le result "210....0", on sait qu'il y a exactement 2 a dans le mot à trouver.
        if (attempt->results[i] == 1) {
            char current_char = attempt->word[i];
            if (current_char == word[i]) {
                // Si on sait qu'une lettre est dans le mot, mais pas à la place n, alors
                // si on la trouve à la place n le mot ne match pas
                return false;
            }
            int min_number_of_occurrence = 0;
            int number_of_occurrence = 0;
            bool has_a_exact_number_of_occurrence = false;
            for (int j = 0; j < len; j++) {
                // On compte cb de fois une lettre doit apparaitre au minimum et on compare au
                // nombre d'apparitions effectives. Si la lettre n'apparait pas assez de
                // fois dans le mot, on renvoie false.
                if (attempt->word[j] == current_char) {
                    if (attempt->results[j] >= 1) {
                        min_number_of_occurrence++;
                    }
                    if (attempt->results[j] == 0) {
                        // Si pour une des occurrences de la lettre dans le mot testé on a un code "0", cela signifie
                        // qu'on peut determiner le nombre exact de fois qu'elle apparait dans le mot.
                        has_a_exact_number_of_occurrence = true;
                    }
                }
                if (word[j] == current_char) {
                    // on compte le nombre occurrence effective dans le mot
                    number_of_occurrence++;
                }
            }
            if ((has_a_exact_number_of_occurrence && min_number_of_occurrence != number_of_occurrence) ||
                min_number_of_occurrence > number_of_occurrence) {
                return false;
            }
        }

        // Si on sait qu'une lettre ne doit pas être dans le mot à une place donnée (code 0)
        // alors on ne peut évidemment pas le match
        if (attempt->results[i] == 0 && attempt->word[i] == word[i]) {
            return false;
        }

    }


    return true;
}

