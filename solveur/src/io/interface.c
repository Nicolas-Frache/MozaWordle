#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "interface.h"

int api_mode = false;

void set_to_api_mode(){
    api_mode = true;
}

void print_hello(){
    if(api_mode){
        printf("api mode !\n");
    } else {
        system("figlet -f slant Solveur ! | cowsay -n");
        printf("\nValentin CHANEL, Matéo DEMANGEON, Nicolas FRACHE, Victor VOISIN.");
        printf("\n-----------------------------------\n\n");
    }
}
