import sqlite3
import string
from project.database import dict_processor

# Emplacement du fichier de la base de données
DB_FILE = "project/database/database.db"

SCHEMA_FILE = "project/database/schema.sql"
DICT_FILE = "project/database/dictionnaire_data.sql"


# Requêtes basiques sur la base de données
def basic_query(sql, param_sql, error_msg):
    try:
        connexion = sqlite3.connect(DB_FILE)
        cursor = connexion.cursor()

        cursor.execute(sql, param_sql)
        query = cursor.fetchall()

        cursor.close()
        connexion.close()
        return query
    except sqlite3.Error as error:
        print(error_msg, error)


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
    lex = dict_processor.get_dict_data()
    print("Insertion des données du dictionnaire dans la base de données...")
    lex.to_sql("dictionnaire", connexion, if_exists="replace")

    # Insertion des données
    cursor.close()
    connexion.close()
    print("** Base de données créée avec succès ! **")
    return None
