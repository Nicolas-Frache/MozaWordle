#include <stdio.h>
#include <assert.h>
#include "attempts_tools.h"

bool is_empty_list_tries(list_attempts_t *pAttempts);

attempt_t *create_attempt(char *words, int *results) {
    attempt_t *new = (attempt_t *) malloc(sizeof(attempt_t));
    new->next = NULL;
    new->word = words;
    new->results = results;
    return new;
}

list_attempts_t *create_list_attempts(int word_size) {
    list_attempts_t *new = (list_attempts_t *) malloc(sizeof(list_attempts_t));
    new->nb_tries = 0;
    new->head = NULL;
    new->word_size = word_size;
    return new;
}

void destroy_attempt(attempt_t *attempt) {
    if (attempt == NULL) return;
    if (attempt->next != NULL) {
        destroy_attempt(attempt->next);
    }
    free(attempt);
}

void destroy_list_attempts(list_attempts_t *list_attempts) {
    destroy_attempt(list_attempts->head);
    free(list_attempts);
}

void append_attempt(list_attempts_t *list_attempts, char *words, int *results) {
    attempt_t *new = create_attempt(words, results);
    if (is_empty_list_attempts(list_attempts)) {
        list_attempts->head = new;
        list_attempts->nb_tries++;
        return;
    }

    attempt_t *current = list_attempts->head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new;
    list_attempts->nb_tries++;
}

bool is_empty_list_attempts(list_attempts_t *pAttempts) {
    return pAttempts->head == NULL;
}

attempt_t *get_list_attempts(list_attempts_t *list_attempts, int index) {
    if (is_empty_list_attempts(list_attempts) || index < 0) return NULL;
    int iterator = index;
    attempt_t *current = list_attempts->head;
    while (iterator > 0) {
        if (current->next == NULL) return NULL;
        current = current->next;
        iterator--;
    }
    return current;
}
