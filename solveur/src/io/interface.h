#ifndef SOLVEUR_INTERFACE_H
#define SOLVEUR_INTERFACE_H

#define file_location "../src/wsolf.txt"

#include <stdbool.h>
/**
 * Affiche le message de bienvenue à l'utilsateur
 */
void print_hello();

/**
 * Affiche message début de loop, avec option pour jouer ou quitter la partie
 * 
 */
void print_begining_msg();


/**
 * Lit dans le fichier wsolf.txt la longueur des mots
 * @return int 
 */
int read_word_length();

int ask_for_word_length();

/**
 * Vérifie que la réponse de type 012 soit du bon format
 * @param res - la réponse de l'utilisateur
 * @param size - la taille attendue
 */
bool is_valid_answer(char *res, int size);

/**
 * Demande à l'utilisateur d'indiquer la solution sous la forme "00102" ou "-1" pour quitter
 * @param tried_word - Le mot que le solveur propose
 * @return - Le tableau rempli des 0 1 2
 */
int *ask_for_answer(char *tried_word);

/**
 * Demande à l'utilisateur s'il veut rejouer
 * @return true si l'utilisateur veut rejouer, false sinon
 */
bool ask_for_play();


#endif //SOLVEUR_INTERFACE_H
