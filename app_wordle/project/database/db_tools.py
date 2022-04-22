from operator import ge
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

def basic_insert(sql,param_sql):
    global cursor
    try:
        connexion = get_db(True)
    except sqlite3.Error as error:
        connexionBaseDeDonneeError()
    try:
        cursor = connexion.cursor()
        cursor.execute(sql, param_sql)
        connexion.commit()
    except sqlite3.Error as error:
        requetageBaseDeDonneeError()
    cursor.close()
    connexion.close()
    return None


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
        if (tables == "utilisateur"):
            new_id = basic_query("SELECT MAX(idUtilisateur) FROM utilisateur", [], disable_dict_factory=True, one_row=True)
        elif (tables == "partie"):
            new_id = basic_query("SELECT MAX(idPartie) FROM partie", [], disable_dict_factory=True, one_row=True)
        elif (tables == "parametre"):
            new_id = basic_query("SELECT MAX(id) FROM parametre", [], disable_dict_factory=True, one_row=True)
        elif (tables == "dictionnaire"):
            new_id = basic_query("SELECT MAX(idMot) FROM dictionnaire", [], disable_dict_factory=True, one_row=True)
        
        if new_id == (None,):
            new_id = [0]

        return new_id[0] + 1
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


## Convention uc = unencrypted, ec = encrypted

def Valid_Inscription(pseudo: str,uc_password : str) -> bool:
    querry = basic_query("SELECT * FROM utilisateur WHERE pseudo =?",(pseudo,),True)
    PseudoAvalaible = querry == []
    ValidPassword = fonctions.ValidPassword(uc_password)

    return PseudoAvalaible and ValidPassword

def Inscription(pseudo: str,uc_password : str) -> None:

    ec_password = fonctions.Encrypt(uc_password)
    idJoueur = generate_max_id("utilisateur")

    basic_insert("INSERT INTO utilisateur (idUtilisateur,pseudo,password) VALUES (?,?,?)",(idJoueur,str(pseudo),str(ec_password)))

    return None


def GoodPassword(pseudo : str, uc_password : str): # -> bool,string:

    querry = basic_query("SELECT password FROM utilisateur WHERE pseudo = ?",(pseudo,),True,True)

    print(querry)

    if (querry == [] or querry == None):
        return False,"Joueur non répertorié dans la base de donnée"
    elif (fonctions.Encrypt(uc_password) == querry[0]):
        return True,"Bon identifiant"
    else :
        return False,"Mauvais identifiant"

def Connect(pseudo : str):
    querry = basic_query("SELECT * FROM utilisateur WHERE pseudo = ?",(pseudo,),True,True)

    print("Info utilisateur :")
    print(querry)

    return querry[0],querry[1],querry[3],querry[4] # renvoie pseudo, paramètre derniere partie et partie en cours


def RegisterGame(motATrouver : string, idParam : int, idJoueur : int = -1) -> int:

    date = basic_query("SELECT CURRENT_TIMESTAMP",[],True,True)[0]
    idPartie = generate_max_id("partie")
    querry = "INSERT INTO partie (idPartie, parametre,estEnCours,idJoueur,motATrouver,date,tourActuel,aGagne,scorePartie) VALUES (?,?,?,?,?,?,?,?,?)"
    param_sql = (idPartie,idParam,1,idJoueur,motATrouver,date,0,0,0)

    basic_insert(querry,param_sql)

    return idPartie

def getIdParam(nb_essais : int, nb_lettres : int, difficulté : int) -> int:
    
    idParam = basic_query("SELECT id FROM parametre WHERE longueur = ? AND nbEssais = ? AND difficulte = ?",(nb_lettres,nb_essais,difficulté),True,True)
   
    if idParam == [] or idParam == None:
        #Creer param
        idParam = generate_max_id("parametre")
        basic_insert("INSERT INTO parametre (id,longueur,nbEssais,difficulte) VALUES (?,?,?,?)",(idParam,nb_lettres,nb_essais,difficulté))
    else : idParam = idParam[0] 
    
    print(f"Id parametre = {idParam}")

    return idParam

def updateCurrentGameUtilisateur(idUtilisateur : int, idPartie : int) -> None:
    
    basic_query("UPDATE utilisateur SET partieEnCours = ? WHERE idUtilisateur = ?",(idPartie,idUtilisateur))
    
    return None