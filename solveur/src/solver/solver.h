struct words_list_t;

#ifndef SOLVEUR_SOLVER_H
#define SOLVEUR_SOLVER_H

#include "tries_list_tools.h"
#include "../dict/dict.h"

/**
 * Pour un mot candidats donnée, on parcourt tous les mots qui pourraient être valides.
 * Puis pour chacun on calcul un score correspondant aux nouvelles informations que nous
 * donnerait le mot à tester dans l'hypothèse ou celui-ci est valide.
 * Enfin, on retourne le score moyen de tous ces mots possibles.
 *
 * @param candidate_word - Le mot dont on veut évaluer le score
 * @param matching_words - La liste (chainée) de mots qui pourraient être valides
 * @param tries - La liste (chainée) des essais precedents permettant d'évaluer le score
 * @return
 */
double evaluate_try_score(char *candidate_word,struct words_list_t *matching_words, struct list_tries_t *tries);

/**
 * Pour un mot candidats donnée, calcul le score correspondant aux nouvelles informations qu'on
 * obtiendrait si le mot matching_word était valide.
 * @param candidate_word - le mot dont on veut évaluer le score dans cette situation
 * @param matching_word - le mot qui pourrait être valide et dont on veut savoir si candidate_word nous
 * donnerait beaucoup d'information si c'est le cas.
 * @param tries - Les essais précèdents pour estimer la quantité de nouvelles informations
 * @return
 */
double evaluate_score_with_specific_combinaison(char *candidate_word, char *matching_word, list_tries_t *tries);


#endif //SOLVEUR_SOLVER_H
