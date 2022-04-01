import os
import sqlite3
import string
import fonctions

from project.exceptions import *
from project.database import dict_creator

# Emplacement du fichier de la base de données
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
DB_FILE = os.path.join(BASE_DIR, "database.db")
DB_FILE_FROM_PROJECT = f"project{os.sep}database{os.sep}" + DB_FILE

SCHEMA_FILE = f"project{os.sep}database{os.sep}schema.sql"
DICT_FILE = f"project{os.sep}database{os.sep}dictionnaire_data.sql"


# Requêtes basiques sur la base de données
def basic_query(sql, param_sql, disable_dict_factory=False, one_row=False):
    global cursor
    try:
        connexion = get_db(disable_dict_factory)
    except sqlite3.Error as error:
        connexionBaseDeDonneeError()
    try:
        cursor = connexion.cursor()
        cursor.execute(sql, param_sql)
        if one_row:
            query = cursor.fetchone()
        else:
            query = cursor.fetchall()
    except sqlite3.Error as error:
        requetageBaseDeDonneeError()
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
        new_id = basic_query("SELECT MAX(id) FROM " + tables, [], one_row=True)

        if new_id is None:
            new_id = [0]

        return new_id + 1
    except sqlite3.Error as error:
        print("Erreur lors de la génération d'un id max : ", error)


def create_db():
    connexion = sqlite3.connect(DB_FILE_FROM_PROJECT)
    cursor = connexion.cursor()

    # Création  des tables
    with open(SCHEMA_FILE) as file:
        sql = file.read()
        cursor.executescript(sql)

    # Récuperation des données du dictionnaire et insertion dans la base de données
    lex = dict_creator.get_dict_data()
    print("Insertion des données du dictionnaire dans la base de données...")
    lex.to_sql("dictionnaire", connexion, if_exists="replace")


## Convention uc = unencrypted, ec = encrypted

def Valid_Inscription(pseudo: str,uc_password : str) -> bool:
    PseudoAvalaible = basic_query("SELECT * FROM utilisateurs WHERE pseudo =?",(pseudo,),"Confirmation de l'unicité du pseudo") == []
    ValidPassword = fonctions.ValidPassword(uc_password)

    return PseudoAvalaible and Valid_Inscription

def Inscription(pseudo: str,uc_password : str) -> None:

    ec_password = fonctions.Encrypt(uc_password)

    basic_query("INSERT INTO utilisateur (pseudo,pwd,parametreDernierePartie,partieEnCours) VALUES (?,?,?)",(pseudo,ec_password,-1,-1),"Erreur lors de l'inscription d'un utilisateur : ")
    return None


def GoodPassword(pseudo : str, uc_password : str): # -> bool,string:

    querry = basic_query("SELECT pwd FROM utilisateur WHERE pseudo = ?",(pseudo,),"Erreur lors de la récupération du mot de passe pour vérifier que celui fourni est bon :")

    if (querry == []):
        return False,"Joueur non répertorié dans la base de donnée"
    elif (fonctions.Encrypt(uc_password) == querry):
        return True,"Bon identifiant"
    else :
        return False,"Mauvais identifiant"

def Connect(pseudo : str):
    querry = basic_query("SELECT * FROM utilisateur WHERE pseudo = ?",(pseudo,),"Erreur lors de la récupération des données lors de la conenxion :")

    return querry[0],querry[2],querry[3] # renvoie pseudo, paramètre derniere partie et partie en cours
