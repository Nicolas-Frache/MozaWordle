#ifndef SOLVEUR_DICT_H
#define SOLVEUR_DICT_H

#include <stdbool.h>
#include "../solver/solver.h"
#include "../solver/attempts_tools.h"


typedef struct word_t {
    char* mots;
    double frequency;
    struct word_t* next;
} word_t;

typedef struct {
    int words_size;
    struct word_t* head;
    int nb_words;
} words_list_t;

/**
 * Génère le dictionnaire de mots d'une longueur donnée
 * à partir de la base de données sqlite3
 * @param word_size
 */
void import_dict(int word_size);

void destroy_dict();

/**
 * Retourne les mots qui pourraient éventuellement être bons parmi un liste,
 * sachant la liste d'essai et de resultats en paramètre
 * @param list_tries - Essai précèdent et leur resultat
 * @param list_tries_t - list des essais precedents et des résultats associées sous formes de liste chainée
 * @return
 */
struct words_list_t *get_all_matching_words(list_attempts_t *list_tries, struct words_list_t *list_words);

/**
 * Teste si un mot peut être valide sachant une liste d'essai en particulier et leurs résultats
 * @param word - mot à tester
 * @param attempts - essais et son résultats
 * @return true si le mot peut être valide, false sinon
 */
bool is_matching_word_specific_attempts(char *word, list_attempts_t *attempts);


/**
 * Test si un mot peut être valide sachant un essai et son résultat
 * @param word - mot à  tester
 * @param attempt - un seul essai
 * @return true si le mot peut être valide, false sinon
 */
bool is_matching_word_one_specific_attempt(char* word, attempt_t* attempt);

#endif //SOLVEUR_DICT_H
