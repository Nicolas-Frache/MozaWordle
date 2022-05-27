#include <stdlib.h>
#include <stdbool.h>

#ifndef SOLVEUR_LIST_TOOLS_H
#define SOLVEUR_LIST_TOOLS_H

typedef struct list_attempts_t {
    int nb_tries;
    struct attempt_t *head;
    int word_size;
} list_attempts_t;

typedef struct attempt_t {
    char *word;
    int *results; //ref a tableau du résultat du match
    struct attempt_t *next;
} attempt_t;

attempt_t* create_attempt(char* words, int* results);

attempt_t* create_attempt_and_result(char *word,char* valid_word);

list_attempts_t *create_list_attempts(int word_size);

void destroy_attempt(attempt_t* attempt);

void destroy_list_attempts(list_attempts_t *list_attempts);

void append_attempt(list_attempts_t *list_attempts, char *words, int *results);

bool is_empty_list_attempts(list_attempts_t *list_attempts);

attempt_t *get_list_attempts(list_attempts_t *list_attempts, int index);

#endif //SOLVEUR_LIST_TOOLS_H
