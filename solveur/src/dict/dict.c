#include <stdio.h>
#include "dict.h"
#define UNUSED(x) (void)(x)

void import_dict(int word_size) {
    UNUSED(word_size);
    // TODO
}

struct words_list_t *get_all_matching_words(list_attempts_t *list_tries, struct words_list_t *list_words) {
    UNUSED(list_tries);
    UNUSED(list_words);
    // TODO
    return NULL;
}

bool is_matching_word_specific_attempts(char *word, list_attempts_t *attempts){
    UNUSED(word);
    UNUSED(attempts);
    if (is_empty_list_attempts(attempts))
        return true;
    
    attempt_t* current = attempts->head;
    bool result = is_matching_word_one_specific_attempt(word, current);
    while (current->next != NULL && result)
    {
        current = current->next;
        result &= is_matching_word_one_specific_attempt(word,current);
    }

    return result;
}

bool is_matching_word_one_specific_attempt(char* word, attempt_t* attempt)
{
    for (int i = 0; i < (int)sizeof(word); i ++) // vérifie d'abord les 2 (les lettres sûr)
    {
        if (attempt->results[i] != 2) { continue;}
        if (word[i] != attempt->word[i]) { return false;}
    }

    for (int i = 0; i < (int)sizeof(word); i ++) // ensuite on vérife les 1
    {
        if (attempt->results[i] != 1) { continue;}

        char char_present = attempt->results[i];
        int number_of_appearance = 1;
        for (int j = i + 1; j < (int)sizeof(word); j++) // on compte cb de fois une lettre doit apparaitre au maximum
        {
            if (attempt->results[j] == 1 && attempt->word[j] == char_present) { number_of_appearance++;}
        }

        for (int j = 0; j < (int)sizeof(word);j++)
        {
            if (word[j] == char_present){ number_of_appearance--;}
        }
        if (number_of_appearance > 0 ) {return false; }
    }

    return true;
}

