#include <stdio.h>
#include <stdbool.h>
#include "interface.h"

int api_mode = false;

void set_to_api_mode(){
    api_mode = true;
}

void print_hello(){
    if(api_mode){
        printf("api mode !\n");
    } else {
        printf("Console mode !\n");
    }
}
