#include "list_tools.h"

try_t* create_try_t(char* words, int* results)
{
    try_t* new = (try_t*)malloc(sizeof(try_t));
    new->next = NULL;
    new->words = words;
    new->results = results;
}

list_tries_t* create_list_tries()
{
    list_tries_t* new = (list_tries_t*)malloc(sizeof(list_tries_t*));
    new->head = NULL;
    new->nb_tries_t = NULL;
}

void destroy_try_t(try_t* try)
{
    if (try->next != NULL)
    {
        destroy_try_t(try->next);
    }
    free(try);
}

void destroy_list_tries(list_tries_t *list_tries)
{
    destroy_try_t(list_tries->head);
    free(list_tries);
}

void append_list_tries(list_tries_t *list_tries, char *words, int *results)
{
    if (is_empty_list_tries(list_tries))
    {
        try_t* new = create_try_t(words,results);
        list_tries->head = new;
    }
    
    try_t* current = list_tries ->head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    try_t* new = create_try_t(words,results);
    current->next= new;
    return;
}

try_t* get_list_tries(list_tries_t *list_tries, int index)
{
    if (is_empty_list_tries(list_tries))
    {
        //handle error
    }
    int iterator = index;
    try_t* current = list_tries ->head;
    while (iterator > 0)
    {
        if (current->next == NULL)
        {
            //handle error
        }
        current = current->next;
        iterator-=1;
    }
    return current;

}

void display_try(try_t* try)
{
    //jsp quoi print
}

void display_list_tries(list_tries_t* list_tries)
{
    if (is_empty_list_tries(list_tries))
    {
        printf("La liste est vide");
    }

    try_t* current = list_tries->head;

    while(current->next != NULL) 
    {
        display_try(current);
    } 
    display_try(current);

    return;
}