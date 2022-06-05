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

// Voir https://en.wikipedia.org/wiki/ANSI_escape_code#SGR_(Select_Graphic_Rendition)_parameters pour les codes couleurs
#define COLOR_BOLD  "\e[1m"
#define COLOR_YELLOW_BOLD  "\e[1;33m"
#define COLOR_RED_BOLD  "\e[1;31m"
#define COLOR_GREEN_BOLD  "\e[1;3;32m"
#define COLOR_GREEN  "\e[32m"
#define COLOR_OFF   "\e[m"
#define COLOR_BOLD_BLUE  "\e[1;34m"


int game_loop(int length);

int main() {
    print_hello();

    int word_size = read_word_length();
    if (word_size < 3 || word_size > 7) {printf("Longueur sélectionnée invalide.\n"); return 0;}
    import_dict(word_size);
    printf("Nombre de mots possible : "COLOR_YELLOW_BOLD"%d\n\n" COLOR_OFF, get_current_possible()->nb_words);
    init_previous_attempts(word_size);
    list_attempts_t *previous_attempts = get_previous_attempt();

    // Sélection du premier mot précalculée pour gagner du tempts
    char *first_words[5] = {"rai", "aria", "aerer", "errera", "recrire"};
    char *first_word = first_words[word_size - 3];
    printf("\n\n● Mots avec la plus grande entropie initiale :   "COLOR_BOLD_BLUE"%s"COLOR_OFF"\n",first_word);
    int *first_result = ask_for_answer(first_word);
    append_attempt(previous_attempts, first_word, first_result);
    update_current_possible_with_attempt();

    while(get_current_possible()->nb_words > 1){
        char *word_to_try = compute_next_best_attempt();
        int *result = ask_for_answer(word_to_try);
        append_attempt(previous_attempts, word_to_try, result);
        update_current_possible_with_attempt();
        printf("Il reste " COLOR_YELLOW_BOLD "%d mots possibles" COLOR_OFF, get_current_possible()->nb_words);
    }
    if(get_current_possible()->nb_words == 0){
        printf(COLOR_RED_BOLD"\n\nLe solveur a perdu:"COLOR_OFF" Le dictionnaire du solveur ne contient aucun mot répondant aux contraintes\n\n");
    } else {
        printf(COLOR_GREEN"\nLa solution est : "COLOR_GREEN_BOLD"%s\n\n\n"COLOR_OFF, get_current_possible()->head->word);
    }

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


