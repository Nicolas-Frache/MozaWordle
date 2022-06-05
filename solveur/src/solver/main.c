#include "../io/interface.h"
#include "../dict/dict.h"
#include <stdio.h>


#ifdef SNOW_ENABLED

#include "../test/snow.h"
#include "../test/test_test.c"
#include "../test/test_attempts_tools.c"
#include "../test/test_dict.c"
snow_main();

#else

int game_loop(int length);

int main() {
    print_hello();

    int word_size = read_word_length();
    if (word_size < 3 || word_size > 7) {printf("Longueur sélectionnée invalide.\n"); return 0;}
    import_dict(word_size);
    init_previous_attempts(word_size);
    list_attempts_t *previous_attempts = get_previous_attempt();

    //char *best_attempt = compute_next_best_attempt(word_size);
    //printf("%s", best_attempt);

    // Sélection du premier mot
    char *first_words[5] = {"rai", "aria", "aerer", "errera", "recrire"};
    char *word_to_try = first_words[word_size - 3];
    int *result = ask_for_answer(word_to_try);
    append_attempt(previous_attempts, word_to_try, result);
    update_current_possible_with_attempt();
    printf("Il reste %d mots possibles", get_current_possible()->nb_words);

    while(get_current_possible()->nb_words > 1){
        char *word_to_try = compute_next_best_attempt();
        int *result = ask_for_answer(word_to_try);
        append_attempt(previous_attempts, word_to_try, result);
        update_current_possible_with_attempt();
        printf("Il reste %d mots possibles", get_current_possible()->nb_words);
    }
    if (get_current_possible()->nb_words > 0)
        printf("\nLa solution est : %s\n", get_current_possible()->head->word);
    else
        printf("\n\nAucun mot du dictionnaire actuel ne correspond ...\nExit\n");

    return 0;
}

#endif
/*

int game_loop(int longueur) {
    //list / tableau mot candidat
    bool stop;
    while (!stop) {
        //récupère mot candidat à partir de mot candidat précédent + nouvelle info

        //calcul meilleur candidat, le renvoie

        //récupère réponse, ajoute info
        //if info == code exit, return exit code
    }

    // eventuellement, msg erreur
    return 0;
}
*/


