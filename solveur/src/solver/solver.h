struct words_list_t;
struct constraints_t;

#ifndef SOLVEUR_SOLVER_H
#define SOLVEUR_SOLVER_H

#include "attempts_tools.h"
#include "../dict/dict.h"

list_attempts_t* get_previous_attempt();

/**
 * A partir de la liste de word qui sont conformes aux indices des tentatives précédentes "matching_words",
 * et de ces tentatives, cette méthode calcule le meilleur mot à proposer pour la tentative suivante
 *
 * @param matching_words - liste de word qui sont conformes aux indices des tentatives précédentes
 * @param constraints - les contraintes
 * @return le prochain mot à proposer
 */
char *compute_next_best_attempt();

/**
 * Pour un mot candidats donné, on parcourt tous les word qui pourraient être valides.
 * Puis pour chacun on calcul un score correspondant aux nouvelles informations que nous
 * donnerait le mot à tester dans l'hypothèse ou celui-ci est valide.
 * Enfin, on retourne le score moyen de tous ces word possibles.
 *
 * @param candidate_word - Le mot dont on veut évaluer le score
 * @param matching_words - La liste (chainée) de word qui pourraient être valides
 * @param constraints - les contraintes
 * @return
 */
double evaluate_attempt_score(char *candidate_word, struct words_list_t *matching_words, struct constraints_t* constraints);

/**
 * Pour un mot candidats et un pattern donnée, calcul le score correspondant aux nouvelles informations qu'on
 * obtiendrait si après avoir saisit le mot condidat on obtient ce pattern
 *
 * @param candidate_word - le mot dont on veut évaluer le score dans cette situation
 * @param matching_word - le mot qui pourrait être valide et dont on veut savoir si candidate_word nous
 * donnerait beaucoup d'information si c'est le cas.
 * @param constraints - les contraintes
 * @return
 */
int evaluate_score_with_specific_combination(char *candidate_word, char *matching_word, struct constraints_t* constraints);

/**
 * Calcule la quantitée d'information gagné en passant des anciennes contraintes aux nouvelles
 * @param old - les contraintes précédentes
 * @param new - les nouvelles contraintes
 */ 
int compute_constraints_improvement(struct constraints_t* old, struct constraints_t* new);

/**
 * Génère tous les patterns 012 pour les mots d'une certaine longueur
 * @param length - longueur des mots
 * @return
 */
int *generate_patterns(int length);

/**
 * Changer un entier de base
 * @param number - entier à convertir
 * @param base - base cible
 * @return - l'entier converti
 */
int convert(int number, int base);

#endif //SOLVEUR_SOLVER_H
