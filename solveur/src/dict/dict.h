#ifndef SOLVEUR_DICT_H
#define SOLVEUR_DICT_H

#include <stdbool.h>
#include "../solver/solver.h"
#include "../solver/attempts_tools.h"

#define NB_LETTERS 26
#define MAX_WORD_LENGTH 20

typedef struct word_t {
    char *word;
    struct word_t *next;
} word_t;

typedef struct words_list_t {
    int words_size;
    int nb_words;
    struct word_t *head, *tail;
} words_list_t;

/**
 * Structure qui décrit les contraintes portant sur l'ensemble du mot à trouver.
 * Et plus particulièrement le nombre minimale ou exact d'une lettre donnée.
 */
typedef struct word_constraint_t {
    int min_nb_occurrences_letters[NB_LETTERS];
    bool is_exact_nb_occurrences_letters[NB_LETTERS];
} word_constraint_t;

/**
 * Structure qui décrit les contraintes du mot à trouver sur une lettre en particulier,
 * Deux possibilité: <lu><li>On connait tout simplement la lettre qui doit être à cet emplacement</li>
 * <li>On a une liste de lettres dont on sait qu'elles ne peuvent pas être à cet emplacement, on représente cela
 * sous forme d'un tableau de 26 booléens</li><lu>
 */
typedef struct emplacement_constraints_t {
    bool has_a_mandatory_letter;
    char mandatory_letter;
    bool forbidden_letters[NB_LETTERS];
} emplacement_constraints_t;

/**
 *  Structure qui permet la description de toutes les contraintes sur le mot à trouver avec :<ul>
 *  <li>Une liste des lettres qui sont totalement interdites dans le word sous forme d'un tableau de 26 booléens</li>
 *  <li>La liste des contraintes qui portent sur le mot dans son ensemble (nombre d'occurrences d'une lettre), sous
 *  forme d'une liste chaînée de structure <i>word_constraint_t</i></li>
 *  <li>Les contraintes qui portent sur un emplacement de mot en particulier (lettre imposée, ou liste de lettres
 *  interdites, sous forme d'un tableau de structures <i>emplacement_constraints_t</i></li></ul>
 */
typedef struct constraints_t {
    int word_size;
    word_constraint_t *word_constraint;
    emplacement_constraints_t *emplacement_constraints;  //tête d'un tableau
    bool global_forbidden_letters[NB_LETTERS];
} constraints_t;

words_list_t* create_word_list(int word_size);
void destroy_word_list(words_list_t* list);
void append_word_list(words_list_t *list, char *word);
words_list_t *get_dictionary();
words_list_t *get_current_possible();
void update_possibilites_w_attempt();

constraints_t *create_constraints(int word_size);
void destroy_constraints(constraints_t *constraints);
void destroy_word_constraint(word_constraint_t *word_constraint);
void destroy_dict();
/**
 * Créé une copie de constraint qui est modifiable sans affecter l'original
 * @param original 
 * @return constraints_t* 
 */
constraints_t* copy_constraints(constraints_t* original);

/**
 * Crée la structure représentant les contraintes associées au mot à trouver à partir de la liste d'essaie
 * @param attempts
 * @return
 */
constraints_t *compute_constraints_from_attempts(list_attempts_t *attempts);


/**
 * Met à jour une contrainte en fonction d'un nouvelle essai.
 * 
 * @param constraints - la contrainte précédente
 * @param attempt - le nouvelle essai
 */
void update_constraints_with_attempts(constraints_t* constraints, attempt_t* attempt);

/**
 * Génère le dictionnaire de word d'une longueur donnée
 * à partir de la base de données sqlite3
 * @param word_size
 */
void import_dict(int word_size);


/**
 * Retourne les word qui sont compatible avec les contraintes passé en arguments
 * @param constraints 
 * @param list_words 
 * @return words_list_t* 
 */
words_list_t* get_all_matching_words(constraints_t* constraints, words_list_t* list_words);

bool is_matching_word_constraints(constraints_t* constraint, char* word);

#pragma region legacy
/**
 * Retourne les word qui pourraient éventuellement être bons parmi un liste,
 * sachant la liste d'essai et de resultats en paramètre
 * @param list_tries - Essai précèdent et leur resultat
 * @param list_tries_t - list des essais precedents et des résultats associées sous formes de liste chainée
 * @return
 */
struct words_list_t *get_all_matching_wordsv0(list_attempts_t *list_tries, struct words_list_t *list_words);

/**
 * Teste si un mot peut être valide sachant une liste d'essai en particulier et leurs résultats
 * @param word - mot à tester
 * @param attempts - essais et son résultats
 * @return true si le mot peut être valide, false sinon
 */
bool is_matching_word_specific_attempts(char *word, list_attempts_t *attempts);


/**
 * Test si un mot peut être valide sachant un essai et son résultat
 * Version pas tout à fait au point et un peu lourde qu'on garde de côté pour éventuellement faire des comparaisons de performance
 * @param word - mot à  tester
 * @param attempt - un seul essai
 * @return true si le mot peut être valide, false sinon
 */
bool is_matching_word_one_specific_attempt_v1(char *word, attempt_t *attempt);
#pragma endregion

/**
 * Test si un mot match avec un ensemble de contraintes liées à des attempts
 * @param word - mot à tester
 * @param constraints - contraintes sur le mot
 * @return vrai si le mot peut être valide, faux sinon
 */
bool is_matching_word_constraints(const char *word, constraints_t *constraints);






#endif //SOLVEUR_DICT_H
