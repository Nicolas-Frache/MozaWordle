#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "dict.h"

#define UNUSED(x) (void)(x)

// Liste de tous les mots du dictionnaire de la longueur qui nous intéresse
words_list_t *dictionary = NULL;
// Liste des mots qui sont possibles avec les indices actuels
words_list_t *current_possibles = NULL;

words_list_t *get_dictionary() { return dictionary; }

words_list_t *get_current_possible() { return current_possibles; }


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

words_list_t *create_word_list(int word_size) {   //NON testé
    words_list_t *new = (words_list_t *) malloc(sizeof(words_list_t));
    new->words_size = word_size;
    new->head = NULL;
    new->tail = NULL;
    new->nb_words = 0;
    return new;
}

word_t *remove_word(words_list_t *list, word_t *to_remove) {
    if (to_remove == list->head) {
        list->head = to_remove->next;
        if (list->head == NULL)
            list->tail = NULL;
        else
            list->head->previous = NULL;

    } else {
        if (to_remove == list->tail)
            list->tail = to_remove->previous;
        if (to_remove->previous != NULL) {
            to_remove->previous->next = to_remove->next;
            if (to_remove->next != NULL)
                to_remove->next->previous = to_remove->previous;
        }
    }
    word_t *res = to_remove->next;
    free(to_remove->word);
    free(to_remove);
    list->nb_words--;
    return res;
}

void destroy_word(word_t *word) {
    if (word->next != NULL) {
        destroy_word(word->next);
    }
    if (word->word != NULL) {
        free(word->word);
    }
    free(word);
}

void destroy_word_list(words_list_t *list) {
    word_t *word = list->head;
    while (word != NULL) {
        word_t *next = word->next;
        free(word->word);
        free(word);
        word = next;
    }
    free(list);
}

void append_word_list(words_list_t *list, char *word) {
    word_t *new_word = (word_t *) malloc(sizeof(word_t));
    new_word->word = word;
    new_word->previous = list->tail;
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

void update_current_possible_with_attempt() {
    words_list_t *liste = get_current_possible();
    constraints_t *constraints = compute_constraints_from_attempts(get_previous_attempt());
    word_t *current = liste->head;
    while (current != NULL) {
        if (!is_matching_word_constraints(current->word, constraints)) {
            current = remove_word(liste, current);
        } else {
            current = current->next;
        }
    }
    destroy_constraints(constraints);
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
        printf(COLOR_RED_BOLD"❗❗ Attention ❗❗" COLOR_OFF
               " , l'instruction d'exécution doit être exactement: "
               COLOR_BOLD_BLUE"'./solver'"COLOR_OFF
               " et PAS './bin/solver' ou autre\n\n");
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

void destroy_dicts() {
    destroy_word_list(dictionary);
    destroy_word_list(current_possibles);
}

words_list_t *get_all_matching_words(constraints_t *constraints, words_list_t *list_words) {
    words_list_t *retour = create_word_list(constraints->word_size);
    retour->words_size = constraints->word_size;
    word_t *last;

    word_t *current = list_words->head;
    while (current->next != NULL) {
        if (is_matching_word_constraints(current->word, constraints)) {
            if (last == NULL) {
                last = current;
            } else {
                last->next = current;
                last = current;
            }
        }
        current = current->next;
    }
    retour->head = last;
    return retour;
}


bool is_matching_word_constraints(const char *word, constraints_t *constraints) {
    int n = constraints->word_size;
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

bool is_matching_word_specific_attempts(char *word, list_attempts_t *attempts) {
    // Méthode servant uniquement pour les tests car l'objectif du système de contraintes est justement
    // de réutiliser les mêmes contraintes à de multiples reprises
    if (is_empty_list_attempts(attempts)) {
        return true;
    }
    constraints_t *constraints = compute_constraints_from_attempts(attempts);
    bool result = is_matching_word_constraints(word, constraints);
    destroy_constraints(constraints);
    return result;
}
