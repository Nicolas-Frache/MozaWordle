#include "../io/interface.h"
#include "../dict/dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <stdbool.h>

int main()
{
    // NON FONCTIONELLE


    print_hello();
    int word_size = read_word_length();
    if (word_size < 3 || word_size > 7) { printf("Longueur sélectionnée invalide.\n"); return EXIT_SUCCESS;}
    import_dict(word_size);

    char *first_words[5] = {"rai", "aria", "aerer", "errera", "recrire"};
    char *word_to_try = first_words[word_size - 3];

    words_list_t* dictionary = get_dictionary();
    word_t* current = dictionary->head;

    int nombre_essai = 0;

    while (current->next != NULL)
    {
        char* word_to_guess = current->word;
        init_previous_attempts(word_size);

        // -----------------------------------------------------
        // ---- init current_possible = copy de dictionnary ----
        // -----------------------------------------------------

        list_attempts_t *previous_attempts = get_previous_attempt();

        int* result = compute_result(word_to_try,word_to_guess);
        append_attempt(previous_attempts, word_to_try, result);
        update_current_possible_with_attempt();
        while(get_current_possible()->nb_words > 1)
        {
            char *word_to_try = compute_next_best_attempt();
            int* result = compute_result(word_to_try,word_to_guess);
            append_attempt(previous_attempts, word_to_try, result);
            update_current_possible_with_attempt();
        }

        if (get_current_possible()->nb_words > 0)
        {
            printf("Pour le mot %s, il a  fallut %d essai(s).\n",word_to_guess,get_previous_attempt()->nb_tries);
            nombre_essai += get_previous_attempt()->nb_tries;
            destroy_list_attempts(get_previous_attempt());
        }
        else 
        {
            printf("Erreur, impossible de deviner un mot ...\n");
            destroy_list_attempts(get_previous_attempt());
            destroy_dicts();
            return EXIT_SUCCESS;
        }

    }

    double score = nombre_essai / get_dictionary()->nb_words;

    printf("\nLe nombre d'essai moyen pour trouver un mot est %f !",score);

    return EXIT_SUCCESS;
}

