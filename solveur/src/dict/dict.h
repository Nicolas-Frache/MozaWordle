#ifndef SOLVEUR_DICT_H
#define SOLVEUR_DICT_H

#include <stdbool.h>
#include "../solver/solver.h"

typedef struct words_list_t {
    int words_size;
    char *mots;
    double frequency;
} dict_t;

/**
 * Génère le dictionnaire de mots d'une longueur donnée
 * à partir de la base de données sqlite3
 * @param word_size
 */
void import_dict(int word_size);

void destroy_dict();

/**
 * Retourne le nombre de mots qui pourraient éventuellement être bon parmis un liste,
 * sachant la liste d'essai et de resultats en paramètre
 * @param list_tries - Essai précèdent et leur resultat
 * @param list_tries_t - list des essais précedents et des résultats associées sous formes de liste chainée
 * @return
 */
struct words_list_t get_all_matching_words(list_tries_t *list_tries, struct words_list_t *list_words);

/**
 * Teste si un mot peut être valide sachant un essai en particulier et son résultat
 * @param word - mot à tester
 * @param try - essai et son résultat
 * @return true si le mot peut être valide, false sinon
 */
bool is_matching_word_specific_try(char *word, try_t try);

/**
 * Pour une liste
 * @param list_tries
 * @param list_words
 * @return
 */
int get_number_of_no_matching_words(list_tries_t *list_tries, struct words_list_t *list_words);

#endif //SOLVEUR_DICT_H
