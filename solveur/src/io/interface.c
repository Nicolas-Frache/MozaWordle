#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "interface.h"

void print_hello() {
    system("figlet -f slant Solveur ! | cowsay -n");
    printf("\nGroupe E32 - Valentin CHANEL, Matéo DEMANGEON, Nicolas FRACHE, Victor VOISIN.");
    printf("\n-----------------------------------\n");
}

void print_begining_msg(){
    int jouer;
    printf("Bienvenue sur RELA !\n");
    jouer = ask_for_play();
    //TODO traiter la cas jouer/quitter
}

int ask_for_word_length() {
    int length = 0;
    printf("Saisissez le nombre de lettre du mot à deviner :\n");
    scanf("%d", &length);
    return length;
}

int read_word_length()
{
    FILE *file = fopen(file_location,"r");
    if (file == NULL)
    {
        printf("Erreur, le fichier n'a pas pu être trouvé avec le chemin : %s\n (Il faut faire la commande dans solveur et pas solveur/bin)\n",file_location);
        return -1;
    }

    int number;
    fscanf(file,"%d",&number);
    printf("Nombre de lettre : %d\n\n",number);
    return number;
}

bool is_valid_answer(int res, int size) {
    // TODO: Vérifier que la chaine est de la bonne longueur, et bien composée de 0, 1 et 2
    return (res >= 0 && res < pow(10, size));
}

void ask_for_answer(int result[], char *tried_word) {
    int taille = strlen(tried_word);
    int res = 0;
    printf("Saisissez le résultat de la proposition : %s\n", tried_word);
    scanf("%d", &res);

    bool is_valid = is_valid_answer(res, taille);

    while (!is_valid) {
        printf("Résultat invalide pour la proposition %s, saisissez-le à nouveau : \n", tried_word);
        scanf("%d", &res);
        is_valid = is_valid_answer(res, taille);
    }

    result = &res;
}

bool ask_for_play(){
    // TODO : Tester
    int jouer = 0;
    printf("Voulez vous jouer ?\n");
    printf("(Tapez 1 pour jouer, tapez 0 pour quitter)\n");
    scanf("%1d\n", &jouer); //scan le premier charactere
    if(jouer!=0 && jouer!=1){  //test si c'est bien un bool
        printf("%1d\n",0);  //%1d prends le premier charactère de l'entrée, à voir si cela pause de problèmes
        return 0;
    }
    printf("%1d\n",jouer);
    return jouer;
}
