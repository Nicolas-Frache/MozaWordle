#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "attempts_tools.h"

#define NB_LETTERS 26

bool is_empty_list_tries(list_attempts_t *pAttempts);

attempt_t *create_attempt(char *words, int *results) {
    attempt_t *new = (attempt_t *) malloc(sizeof(attempt_t));
    new->next = NULL;
    new->word = words;
    new->results = results;
    return new;
}

attempt_t* create_attempt_and_result(char *word,const char* valid_word)
{
    attempt_t *new = (attempt_t *) malloc(sizeof(attempt_t));
    new->next = NULL;
    new->word = word;
    int n = (int)strlen(word);
    int result[n];
    int nb_of_occurence[NB_LETTERS];
    for (int i = 0; i < NB_LETTERS; i++) {nb_of_occurence[i] = 0;} // initialise array a 0 ( et évite memory leak )

    for (int i = 0; i < n; i++)
    {
        int indice_letter = valid_word[i] - 97;
        nb_of_occurence[indice_letter]++;
    }

    for (int i = 0; i < n; i++) 
    {
        int indice_letter = word[i] - 97;
        if (word[i] == valid_word[i])
        {
            result[i] = 2;
            nb_of_occurence[indice_letter]--;
        }
        else if (nb_of_occurence[indice_letter] > 0)
        {
            result[i] = 1;
            nb_of_occurence[indice_letter]--;
        }
        else {result[i] = 0;}
    }
    new->results = result;
    return new;
}

int* compute_result(char* candidate, char* valid_word)
{
    int n = (int)strlen(candidate);
    int *result = calloc(1, sizeof(int) * n);
    int nb_of_occurence[NB_LETTERS] = {};

    for (int i = 0; i < n; i++)
    {
        int indice_letter = valid_word[i] - 97;
        nb_of_occurence[indice_letter]++;
    }

    for (int i = 0; i < n; i++) 
    {
        int indice_letter = candidate[i] - 97;
        if (candidate[i] == valid_word[i])
        {
            result[i] = 2;
            nb_of_occurence[indice_letter]--;
        }
        else if (nb_of_occurence[indice_letter] > 0)
        {
            result[i] = 1;
            nb_of_occurence[indice_letter]--;
        }
        else {result[i] = 0;}
    }
    return result;
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
    if (list_attempts == NULL) return;
    if (list_attempts->head != NULL)
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

attempt_t *remove_attempt(list_attempts_t *list, attempt_t *to_remove){
    if(is_empty_list_attempts(list))
        return NULL;
    if(list->head == to_remove){
        list->head = to_remove->next;
    } else {
        attempt_t *current = list->head;
        while(current->next != to_remove){
            current = current->next;
        }
        current->next = to_remove->next;
    }
    list->nb_tries--;
    attempt_t *res = to_remove->next;
    free(to_remove);
    return res;
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
