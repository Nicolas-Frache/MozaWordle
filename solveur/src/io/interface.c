#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "interface.h"

#define COLOR_OFF   "\e[m"
#define COLOR_BOLD  "\e[1m"
#define BOLD_BLUE    "\033[1;34m"
#define COLOR_BOLD_SLOW_BLINKING_RED  "\e[1;6;33m"

void print_hello() {
    //system("figlet -f slant Solveur ! | cowsay -n");
    printf("\n ______________________________________________\n"
           "/    _____       __                         __ \\\n"
           "|   / ___/____  / /   _____  __  _______   / / |\n"
           "|   \\__ \\/ __ \\/ / | / / _ \\/ / / / ___/  / /  |\n"
           "|  ___/ / /_/ / /| |/ /  __/ /_/ / /     /_/   |\n"
           "| /____/\\____/_/ |___/\\___/\\____/_/     (_)    |\n"
           "\\                                              /\n"
           " ----------------------------------------------\n"
           "        \\   ^__^\n"
           "         \\  (" COLOR_BOLD_SLOW_BLINKING_RED "oo" COLOR_OFF ")\\_______\n"
           "            (__)\\       )\\/\\\n"
           "                ||----w |\n"
           "                ||     ||");

    printf("\nGroupe E32 - Valentin CHANEL, Matéo DEMANGEON, Nicolas FRACHE, Victor VOISIN.");
    printf("\n-----------------------------------\n");
}

int read_word_length() {
    FILE *file = fopen(file_location, "r");
    if (file == NULL) {
        printf("Erreur, le fichier n'a pas pu être trouvé avec le chemin : %s\n (Il faut faire la commande dans solveur et pas solveur/bin)\n",
               file_location);
        return -1;
    }

    int number;
    fscanf(file, "%d", &number);
    printf("Nombre de lettre : %d\n", number);
    return number;
}

bool is_valid_answer(char *res, int size) {
    if ((int) strlen(res) != size) { return false; }
    for (int i = 0; i < size; i++) {
        if (res[i] != '0' && res[i] != '1' && res[i] != '2') { return false; }
    }
    return true;
}

int *ask_for_answer(char *tried_word) {
    int taille = strlen(tried_word);
    int *table = calloc(1, sizeof(int) * taille);
    char str[31];
    printf("Saisissez le résultat de la proposition\n");
    printf(BOLD_BLUE "> " COLOR_OFF);
    scanf("%30s", str);
    if (strcmp(str, "-1") == 0) {
        printf("Arrêt du solveur.\n");
        table[0] = -1;
        return table;
    }

    bool is_valid = is_valid_answer(str, taille);
    while (!is_valid) {
        printf("Résultat invalide pour la proposition " BOLD_BLUE"%s" COLOR_OFF ", saisissez-le à nouveau (ou -1 pour quitter) : \n",
               tried_word);
        printf(BOLD_BLUE "> " COLOR_OFF);
        scanf("%30s", str);
        if (strcmp(str, "-1") == 0) {
            printf("Arrêt du solveur.\n");
            table[0] = -1;
            return table;
        }
        is_valid = is_valid_answer(str, taille);
    }

    // Conversion en tableau
    for (int i = 0; i < (int) strlen(str); i++) { table[i] = str[i] - 48; }

    return table;
}

bool ask_for_play() {
    int jouer = 0;
    printf("Voulez vous jouer ?\n");
    printf("(Tapez 1 pour jouer, tapez 0 pour quitter)\n");
    scanf("%1d\n", &jouer); //scan le premier charactere
    if (jouer != 0 && jouer != 1) {  //test si c'est bien un bool
        printf("%1d\n", 0);  //%1d prends le premier charactère de l'entrée, à voir si cela pause de problèmes
        return 0;
    }
    printf("%1d\n", jouer);
    return jouer;
}
