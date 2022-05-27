struct words_list_t;

#ifndef SOLVEUR_SOLVER_H
#define SOLVEUR_SOLVER_H

#include "attempts_tools.h"
#include "../dict/dict.h"


/**
 * A partir de la liste de word qui sont conformes aux indices des tentatives précédentes "matching_words",
 * et de ces tentatives, cette méthode calcule le meilleur mot à proposer pour la tentative suivante
 *
 * @param matching_words - liste de word qui sont conformes aux indices des tentatives précédentes
 * @param attempts - liste de tentatives précédentes
 * @return le prochain mot à proposer
 */
char *compute_next_best_attempt(struct words_list_t *matching_words, struct list_attempts_t *attempts);

/**
 * Pour un mot candidats donné, on parcourt tous les word qui pourraient être valides.
 * Puis pour chacun on calcul un score correspondant aux nouvelles informations que nous
 * donnerait le mot à tester dans l'hypothèse ou celui-ci est valide.
 * Enfin, on retourne le score moyen de tous ces word possibles.
 *
 * @param candidate_word - Le mot dont on veut évaluer le score
 * @param matching_words - La liste (chainée) de word qui pourraient être valides
 * @param tries - La liste (chainée) des essais precedents permettant d'évaluer le score
 * @return
 */
double evaluate_attempt_score(char *candidate_word, struct words_list_t *matching_words, struct list_attempts_t *tries);

/**
 * Pour un mot candidats donnée, calcul le score correspondant aux nouvelles informations qu'on
 * obtiendrait si le mot matching_word était valide.
 *
 * Dans un premier temps on peut renvoyer 1 si le mot ne match plus avec les indices, 0 sinon
 *
 * @param candidate_word - le mot dont on veut évaluer le score dans cette situation
 * @param matching_word - le mot qui pourrait être valide et dont on veut savoir si candidate_word nous
 * donnerait beaucoup d'information si c'est le cas.
 * @param tries - Les essais precedents pour estimer la quantité de nouvelles informations
 * @return
 */
double evaluate_score_with_specific_combination(char *candidate_word, char *matching_word, list_attempts_t *tries);


#endif //SOLVEUR_SOLVER_H
