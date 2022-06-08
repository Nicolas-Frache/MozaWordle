#ifndef SOLVEUR_INTERFACE_H
#define SOLVEUR_INTERFACE_H

#define file_location "../src/wsolf.txt"

#define COLOR_OFF   "\e[m"
#define BOLD_BLUE    "\033[1;34m"
#define COLOR_RED_BOLD  "\e[1;31m"
#define COLOR_BOLD_SLOW_BLINKING_YELLOW  "\e[1;6;33m"
#define COLOR_BOLD_SLOW_BLINKING_RED  "\e[1;6;31m"
#define COLOR_BOLD_BLUE  "\e[1;34m"

#include <stdbool.h>

/**
 * Affiche le message de bienvenue à l'utilsateur
 */
void print_hello();

/**
 * Lit dans le fichier wsolf.txt la longueur des mots
 * @return int 
 */
int read_word_length();

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

#endif //SOLVEUR_INTERFACE_H
