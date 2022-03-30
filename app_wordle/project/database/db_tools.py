import os
import sqlite3
import string
from project.database import dict_creator

# Emplacement du fichier de la base de données
DB_FILE = "database.db"
DB_FILE_FROM_PROJECT = f"project{os.sep}database{os.sep}" + DB_FILE

SCHEMA_FILE = f"project{os.sep}database{os.sep}schema.sql"
DICT_FILE = f"project{os.sep}database{os.sep}dictionnaire_data.sql"


# Requêtes basiques sur la base de données
def basic_query(sql, param_sql, disable_dict_factory=False):
    connexion = get_db(disable_dict_factory)
    cursor = connexion.cursor()

    cursor.execute(sql, param_sql)
    query = cursor.fetchall()

    cursor.close()
    connexion.close()
    return query


def get_db(disable_dict_factory=False):
    db = sqlite3.connect(DB_FILE)
    if not disable_dict_factory:
        db.row_factory = dict_factory
    return db


# Voir: https://docs.python.org/3/library/sqlite3.html#sqlite3.Connection.row_factory
def dict_factory(cursor, row):
    d = {}
    for idx, col in enumerate(cursor.description):
        d[col[0]] = row[idx]
    return d


def generate_max_id(tables: string) -> int:
    try:
        connexion = sqlite3.connect(DB_FILE)
        cursor = connexion.cursor()

        cursor.execute("SELECT max(id) FROM " + tables)  # TODO: Rendre robuste aux injections sql
        new_id = cursor.fetchone()

        if new_id == None:
            new_id = [0]

        cursor.close()
        connexion.close()
        return new_id + 1
    except sqlite3.Error as error:
        print("Erreur lors de la génération d'un id max : ", error)


def create_db():
    connexion = sqlite3.connect(DB_FILE)
    cursor = connexion.cursor()

    # Création  des tables
    with open(SCHEMA_FILE) as file:
        sql = file.read()
        cursor.executescript(sql)

    # Récuperation des données du dictionnaire et insertion dans la base de données
    lex = dict_creator.get_dict_data()
    print("Insertion des données du dictionnaire dans la base de données...")
    lex.to_sql("dictionnaire", connexion, if_exists="replace")

    # Insertion des données
    cursor.close()
    connexion.close()
    print("** Base de données créée avec succès ! **")
    return None
