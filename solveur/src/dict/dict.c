#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "dict.h"

#define UNUSED(x) (void)(x)

// Liste de tous les mots du dictionnaire de la longueur qui nous intéresse
words_list_t *dictionary = NULL;
// Liste des mots qui sont possibles avec les indices actuels
words_list_t *current_possibles = NULL;

words_list_t *get_dictionary(){return dictionary;}
words_list_t *get_current_possible(){return current_possibles;}

constraints_t *create_constraints(int word_size) {
    constraints_t *constraints = calloc(1, sizeof(constraints_t));
    constraints->emplacement_constraints = calloc(1, sizeof(emplacement_constraints_t) * word_size);
    constraints->word_constraint = calloc(1, sizeof(word_constraint_t));
    constraints->word_size = word_size;
    return constraints;
}

void destroy_constraints(constraints_t *constraints) {
    free(constraints->emplacement_constraints);
    free(constraints->word_constraint);
    free(constraints);
}

constraints_t *copy_constraints(constraints_t *original) {
    UNUSED(original);
    //TODO
    return original;
}

words_list_t *create_word_list(int word_size) {   //NON testé
    words_list_t *new = (words_list_t *) malloc(sizeof(words_list_t));
    new->words_size = word_size;
    new->head = NULL;
    new->tail = NULL;
    new->nb_words = 0;
    return new;
}

void destroy_word(word_t *word) {
    if (word->next != NULL) {
        destroy_word(word->next);
    }
    free(word);
}

void destroy_word_list(words_list_t *list) {
    if (list->head != NULL) {
        destroy_word(list->head);
    }
    free(list);
}

void append_word_list(words_list_t *list, char *word) {
    word_t *new_word = (word_t *) malloc(sizeof(word_t));
    new_word->word = word;
    new_word->next = NULL;
    if (list->head == NULL) {
        list->head = new_word;
        list->tail = new_word;
    } else {
        list->tail->next = new_word;
        list->tail = new_word;
    }
    list->nb_words++;
}


constraints_t *compute_constraints_from_attempts(list_attempts_t *attempts) {
    attempt_t *attempt = attempts->head;
    constraints_t *constraints = create_constraints(attempts->word_size);
    while (attempt != NULL) {
        // Prise en compte de l'essai
        // On initialise le nombre minimum de chaque lettre à 0
        int min_nb_occurrences_letters[NB_LETTERS] = {};
        bool can_be_exact_or_forbidden[NB_LETTERS] = {};
        // On parcourt le mot de gauche à droite
        for (int i = 0; i < constraints->word_size; i++) {
            int indice_lettre_attempt = attempt->word[i] - 97;
            if (!constraints->emplacement_constraints[i].has_a_mandatory_letter) {
                // On récolte les informations ne concernant pas le nombre de lettres dans un mot
                if (attempt->results[i] == 2) {
                    constraints->emplacement_constraints[i].has_a_mandatory_letter = true;
                    constraints->emplacement_constraints[i].mandatory_letter = attempt->word[i];
                }
                if (attempt->results[i] <= 1) {
                    constraints->emplacement_constraints[i].forbidden_letters[indice_lettre_attempt] = true;
                }
            }
            // Puis si le code est 2 ou 1, on augmente de 1 le nombre d'occurrences minimal
            if (attempt->results[i] >= 1) {
                min_nb_occurrences_letters[indice_lettre_attempt]++;
            }
            // Si on a un code 0, mais un nombre minimal d'essai non-nul, alors on sait que le nombre minimal
            // qu'on trouvera est en réalité le nombre exact d'occurrences de la lettre
            if (attempt->results[i] == 0) {
                can_be_exact_or_forbidden[indice_lettre_attempt] = true;
            }
        }

        // On parcourt maintenant les informations qu'on a sur les nombres d'occurrences de chaque lettre
        // pour déterminer si on a de nouvelles informations, si c'est le cas, on le sauvegarde
        for (int i = 0; i < NB_LETTERS; i++) {
            // Si on ne connait pas déjà le nombre exact...
            if (!constraints->word_constraint->is_exact_nb_occurrences_letters[i]) {
                // ... Alors s'il y a eu un code zero associé à une lettre qui a de part ailleurs
                // un code != 0 dans le mot, on sait qu'on a connaissance du nombre exact d'occurrences.
                if (can_be_exact_or_forbidden[i] && min_nb_occurrences_letters[i] != 0) {
                    constraints->word_constraint->is_exact_nb_occurrences_letters[i] = true;
                    constraints->word_constraint->min_nb_occurrences_letters[i] = min_nb_occurrences_letters[i];
                } else if (constraints->word_constraint->min_nb_occurrences_letters[i] <
                           min_nb_occurrences_letters[i]) {
                    // Et à défaut si notre nombre min et supérieur à celui enregistré, alors on gagne
                    // de l'information, on l'enregistre donc
                    constraints->word_constraint->min_nb_occurrences_letters[i] = min_nb_occurrences_letters[i];
                }
            }

            // De plus, on a précédemment mis de côté dans "can_be_exact_or_forbidden" les lettres avec un code 0
            // Donc possiblement absentes du mot. On s'en assure en vérifiant mtn que le nb d'occurence est 0
            // Si c'est le cas on sauvegarde l'information dans la structure
            if (can_be_exact_or_forbidden[i] && min_nb_occurrences_letters[i] == 0) {
                constraints->global_forbidden_letters[i] = true;
            }
        }
        attempt = attempt->next;
    }
    return constraints;
}

void update_constraints_with_attempts(constraints_t *old_constraints, attempt_t *attempt) {
    // NON TESTE
    constraints_t *new_constraints = copy_constraints(old_constraints);

    int min_nb_occurrences_letters[26];
    memcpy(min_nb_occurrences_letters, new_constraints->word_constraint->min_nb_occurrences_letters,
           sizeof(min_nb_occurrences_letters));
    bool can_be_exact_or_forbidden[26];
    memcpy(can_be_exact_or_forbidden, new_constraints->word_constraint->is_exact_nb_occurrences_letters,
           sizeof(can_be_exact_or_forbidden));

    for (int i = 0; i < new_constraints->word_size; i++) {
        int indice_lettre_attempt = attempt->word[i] - 97;
        if (!new_constraints->emplacement_constraints[i].has_a_mandatory_letter) {
            // On récolte les informations ne concernant pas le nombre de lettres dans un mot
            if (attempt->results[i] == 2) {
                new_constraints->emplacement_constraints[i].has_a_mandatory_letter = true;
                new_constraints->emplacement_constraints[i].mandatory_letter = attempt->word[i];
            }
            if (attempt->results[i] <= 1) {
                new_constraints->emplacement_constraints[i].forbidden_letters[indice_lettre_attempt] = true;
            }
        }
        // Puis si le code est 2 ou 1, on augmente de 1 le nombre d'occurrences minimal
        if (attempt->results[i] >= 1) {
            min_nb_occurrences_letters[indice_lettre_attempt]++;
        }
        // Si on a un code 0, mais un nombre minimal d'essai non-nul, alors on sait que le nombre minimal
        // qu'on trouvera est en réalité le nombre exact d'occurrences de la lettre
        if (attempt->results[i] == 0) {
            can_be_exact_or_forbidden[indice_lettre_attempt] = true;
        }
    }

    // On parcourt maintenant les informations qu'on a sur les nombres d'occurrences de chaque lettre
    // pour déterminer si on a de nouvelles informations, si c'est le cas, on le sauvegarde
    for (int i = 0; i < 26; i++) {
        // Si on ne connait pas déjà le nombre exact...
        if (!new_constraints->word_constraint->is_exact_nb_occurrences_letters[i]) {
            // ... Alors s'il y a eu un code zero associé à une lettre qui a de part ailleurs
            // un code != 0 dans le mot, on sait qu'on a connaissance du nombre exact d'occurrences.
            if (can_be_exact_or_forbidden[i] && min_nb_occurrences_letters[i] != 0) {
                new_constraints->word_constraint->is_exact_nb_occurrences_letters[i] = true;
                new_constraints->word_constraint->min_nb_occurrences_letters[i] = min_nb_occurrences_letters[i];
            } else if (new_constraints->word_constraint->min_nb_occurrences_letters[i] <
                       min_nb_occurrences_letters[i]) {
                // Et à défaut si notre nombre min et supérieur à celui enregistré, alors on gagne
                // de l'information, on l'enregistre donc
                new_constraints->word_constraint->min_nb_occurrences_letters[i] = min_nb_occurrences_letters[i];
            }
        }
        // De plus, on a précédemment mis de côté dans "can_be_exact_or_forbidden" les lettres avec un code 0
        // Donc possiblement absentes du mot. On s'en assure en vérifiant mtn que le nb d'occurence est 0
        // Si c'est le cas on sauvegarde l'information dans la structure
        if (can_be_exact_or_forbidden[i] && min_nb_occurrences_letters[i] == 0) {
            new_constraints->global_forbidden_letters[i] = true;
        }
    }
}

/**
 * This callback_save_dict_in_struct provides a way to obtain results from SELECT statements
 * @see{https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm}
 * @param data - Data provided in the 4th argument of sqlite3_exec()
 * @param argc - The number of columns in row
 * @param argv - An array of strings representing fields in the row
 * @param azColName - An array of strings representing column names
 * @return
 */
static int callback_save_dict_in_struct(void *data, int argc, char **argv, char **azColName) {
    /*for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }*/
    UNUSED(data);
    UNUSED(argc);
    UNUSED(azColName);

    char *word = malloc(sizeof(char) * (strlen(argv[0]) + 1));
    strcpy(word, argv[0]);
    append_word_list(dictionary, word);
    if (strcmp(argv[1], "1") == 0) {
        char *word_2 = malloc(sizeof(char) * (strlen(argv[0]) + 1));
        strcpy(word_2, argv[0]);
        append_word_list(current_possibles, word_2);
    }
    return 0;
}

void import_dict(int word_size) {
    // Création des listes de mots
    dictionary = create_word_list(word_size);
    current_possibles = create_word_list(word_size);

    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    const char *data = "Callback function called";
    /* Ouverture de la db en mode lecture seule */
    rc = sqlite3_open("file:../src/dict/database.db?mode=ro&cache=private", &db);
    if (rc != 0) {
        fprintf(stderr, "Erreur avec base de donnée: %s\n", sqlite3_errmsg(db));
        printf("❗❗ Attention ❗❗ , l'instruction d'exécution doit être exactement: './solver' et PAS './bin/solver' ou autre\n\n");
        sqlite3_close(db);
        return;
    }

    /* Create SQL statement */
    char sql[100];
    snprintf(sql, sizeof(sql), "SELECT mot, motDevinable FROM dictionnaire WHERE longueur = %d;", word_size);
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback_save_dict_in_struct, (void *) data, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
}

words_list_t *get_all_matching_wordsv2(constraints_t *constraints, words_list_t *list_words) {
    //NON testé
    words_list_t *retour = create_word_list(constraints->word_size);
    retour->words_size = constraints->word_size;
    word_t *last;

    word_t *current = list_words->head;
    while (current->next != NULL) {
        if (is_matching_word_constraints(current->word, constraints)) {
            if (last == NULL) { last = current; }
            else {
                last->next = current;   // je crois que ca pose pas de probleme pour itérer sur list_words
                last = current;
            }
        }
        current = current->next;

    }
    retour->head = last;
    return retour;
}

struct words_list_t *get_all_matching_words(list_attempts_t *list_tries, struct words_list_t *list_words) {
    UNUSED(list_tries);
    UNUSED(list_words);
    // TODO
    return NULL;
}

bool is_matching_word_specific_attempts(char *word, list_attempts_t *attempts) {
    if (is_empty_list_attempts(attempts))
        return true;

    // Méthode servant uniquement pour les tests car l'objectif du système de contraintes et justement
    // de réutiliser les mêmes contraintes à de multiples reprises
    constraints_t *constraints = compute_constraints_from_attempts(attempts);
    bool result = is_matching_word_constraints(word, constraints);
    destroy_constraints(constraints);
    return result;

    // Méthode précèdente, qu'on garde de côté en vue d'éventuels comparaison de performances à venir
    /*
    attempt_t *current = attempts->head;
    bool result = is_matching_word_one_specific_attempt_v1(word, current);
    while (current->next != NULL && result) {
        current = current->next;
        result &= is_matching_word_one_specific_attempt_v1(word, current);
    }
    return result;
     */
}

bool is_matching_word_constraints(const char *word, constraints_t *constraints) {
    int n = constraints->word_size;
    if (n != (int) strlen(word)) return false;
    int compteur_lettre[NB_LETTERS] = {};
    for (int i = 0; i < n; i++) {
        int indice_lettre = word[i] - 97;
        // Test des contraintes pour chaque lettre séparement
        if (
            // Test des lettres obligatoires
                (constraints->emplacement_constraints[i].has_a_mandatory_letter &&
                 constraints->emplacement_constraints[i].mandatory_letter != word[i])
                // Test de la présence d'une lettre interdite pour l'ensemble du mot
                || (constraints->global_forbidden_letters[indice_lettre])
                // Test de la présence d'une lettre interdite pour cet index en particulier
                || (constraints->emplacement_constraints[i].forbidden_letters[indice_lettre])) {
            return false;
        }
        // On garde le comptage de lettre pour l'étape suivante
        compteur_lettre[indice_lettre]++;
    }
    // Pour finir on vérifie que les contraintes concernant le nombre de lettre sont respectées
    for (int i = 0; i < NB_LETTERS; i++) {
        if (
            // Test sur le nombre minimun d'occurence de la lettre
                (compteur_lettre[i] < constraints->word_constraint->min_nb_occurrences_letters[i])
                // Test si le nombre exact d'occcurence est connu
                || (constraints->word_constraint->is_exact_nb_occurrences_letters[i]
                    && compteur_lettre[i] != constraints->word_constraint->min_nb_occurrences_letters[i])
                ) {
            return false;
        }
    }
    return true;
}

#pragma region legacy


/**
 * Version pas tout à fait au point et un peu lourde qu'on garde de côté pour éventuellement faire des comparaisons de performance
 */
bool is_matching_word_one_specific_attempt_v1(char *word, attempt_t *attempt) {
    int len = (int) strlen(word);
    for (int i = 0; i < len; i++) {

        // On vérifie que toute les lettres dont connait la place directement (code 2),
        // sont bien au bon endroit
        if (attempt->results[i] == 2 && word[i] != attempt->word[i]) {
            return false;
        }

        // On vérifie que toutes les lettres dont on sait qu'elles sont à la mauvaise place (code 1),
        // sont bien présentes en bon nombre dans le mot.
        // Par exemple : Si on a le mot "aaa...." et le result "210....0", on sait qu'il y a exactement 2 a dans le mot à trouver.
        if (attempt->results[i] == 1) {
            char current_char = attempt->word[i];
            if (current_char == word[i]) {
                // Si on sait qu'une lettre est dans le mot, mais pas à la place n, alors
                // si on la trouve à la place n le mot ne match pas
                return false;
            }
            int min_number_of_occurrence = 0;
            int number_of_occurrence = 0;
            bool has_a_exact_number_of_occurrence = false;
            for (int j = 0; j < len; j++) {
                // On compte cb de fois une lettre doit apparaitre au minimum et on compare au
                // nombre d'apparitions effectives. Si la lettre n'apparait pas assez de
                // fois dans le mot, on renvoie false.
                if (attempt->word[j] == current_char) {
                    if (attempt->results[j] >= 1) {
                        min_number_of_occurrence++;
                    }
                    if (attempt->results[j] == 0) {
                        // Si pour une des occurrences de la lettre dans le mot testé on a un code "0", cela signifie
                        // qu'on peut determiner le nombre exact de fois qu'elle apparait dans le mot.
                        has_a_exact_number_of_occurrence = true;
                    }
                }
                if (word[j] == current_char) {
                    // on compte le nombre occurrence effective dans le mot
                    number_of_occurrence++;
                }
            }
            if ((has_a_exact_number_of_occurrence && min_number_of_occurrence != number_of_occurrence) ||
                min_number_of_occurrence > number_of_occurrence) {
                return false;
            }
        }

        // Si on sait qu'une lettre ne doit pas être dans le mot à une place donnée (code 0)
        // alors on ne peut évidemment pas le match
        if (attempt->results[i] == 0 && attempt->word[i] == word[i]) {
            return false;
        }

    }


    return true;
}

#pragma endregion
