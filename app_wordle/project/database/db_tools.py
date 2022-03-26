import sqlite3
import string

# Emplacement du fichier de la base de données
DB_FILE = "project/database/database.db"
SCHEMA_FILE = "project/database/schema.sql"


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

        cursor.execute("SELECT max(id) FROM " + tables) # TODO: Rendre robuste aux injections sql
        new_id = cursor.fetchone()

        if new_id == None:
            new_id = [0]

        cursor.close()
        connexion.close()
        return new_id + 1
    except sqlite3.Error as error:
        print("Erreur lors de la génération d'un id max : ", error)


def create_db():
    try:
        connexion = sqlite3.connect(DB_FILE)
        cursor = connexion.cursor()

        # Création  des tables
        with open(SCHEMA_FILE) as file:
            sql = file.read()
            cursor.executescript(sql)

        # Insertion des données
        cursor.close()
        connexion.close()
        return None
    except sqlite3.Error as error:
        print("Erreur lors de la création de DB : ", error)
