#ifndef SOLVEUR_DICT_H
#define SOLVEUR_DICT_H

#include <stdbool.h>
#include "../solver/solver.h"
#include "../solver/attempts_tools.h"

typedef struct {
    int word_size;
    char* mots;
    double frequency;
} word_t;

typedef struct {
    int words_size;
    word_t** words;
} words_list_t;

/**
 * Génère le dictionnaire de mots d'une longueur donnée
 * à partir de la base de données sqlite3
 * @param word_size
 */
void import_dict(int word_size);

void destroy_dict();

/**
 * Retourne les mots qui pourraient éventuellement être bon parmi un liste,
 * sachant la liste d'essai et de resultats en paramètre
 * @param list_tries - Essai précèdent et leur resultat
 * @param list_tries_t - list des essais precedents et des résultats associées sous formes de liste chainée
 * @return
 */
struct words_list_t get_all_matching_words(list_attempts_t *list_tries, struct words_list_t *list_words);

/**
 * Teste si un mot peut être valide sachant un essai en particulier et son résultat
 * @param word - mot à tester
 * @param try - essai et son résultat
 * @return true si le mot peut être valide, false sinon
 */
bool is_matching_word_specific_attempts(char *word, list_attempts_t *attempts);

#endif //SOLVEUR_DICT_H
