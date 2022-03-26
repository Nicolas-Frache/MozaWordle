import sqlite3
import string
import fonctions

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

    querry = basic_query("SELECT pwd FROM utilisateur WHERE pseudo = ?",(pseudo,),"Erreur lors de la écupération du mot de passe pour vérifier que celui fourni est bon :")

    if (querry == []):
        return False,"Joueur non répertorié dans la base de donnée"
    elif (fonctions.Encrypt(uc_password) == querry):
        return True,"Bon identifiant"
    else :
        return False,"Mauvais identifiant"

def Connect(pseudo : str):
    querry = basic_query("SELECT * FROM utilisateur WHERE pseudo = ?",(pseudo,),"Erreur lors de la récupération des données lors de la conenxion :")

    return querry[0],querry[2],querry[4] # renvoie pseudo, paramètre derniere partie et partie en cours