//
// Created by nicol on 09/05/2022.
//
#include <stdlib.h>
#include <stdbool.h>

#ifndef SOLVEUR_LIST_TOOLS_H
#define SOLVEUR_LIST_TOOLS_H

typedef struct list_tries_t {
    int nb_tries_t;
    struct try_t *head;
} list_tries_t;

typedef struct try_t {
    char* words;
    int* results; //ref a tableau du résultat du match
    struct try_t *next;
} try_t;

list_tries_t *create_list_tries();

void destroy_list_tries(list_tries_t *list_tries);

void append_list_tries(list_tries_t *list_tries, char *words, int *results);

bool is_empty_list_tries(list_tries_t* list_tries);

try_t *get_list_tries(list_tries_t *list_tries, int index);

void display_list_tries(list_tries_t *list_tries);

#endif //SOLVEUR_LIST_TOOLS_H
