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
def basic_query(sql, param_sql, disable_dict_factory=False, one_row=False, commit=False):
    global cursor
    try:
        connexion = get_db(disable_dict_factory)
    except sqlite3.Error as error:
        connexionBaseDeDonneeError(str(error))
        return
    try:
        cursor = connexion.cursor()
        cursor.execute(sql, param_sql)
        if one_row:
            query = cursor.fetchone()
        else:
            query = cursor.fetchall()
        if commit:
            connexion.commit()
    except sqlite3.Error as error:
        requetageBaseDeDonneeError(str(error))
        return
    finally:
        cursor.close()
        connexion.close()
    return query


def basic_insert(sql, param_sql):
    try:
        basic_query(sql, param_sql, commit=True)
    except sqlite3.Error as error:
        insertionBaseDeDonneeError(str(error))


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

        if tables == "utilisateur":
            new_id = basic_query("SELECT MAX(idUtilisateur) FROM utilisateur", [], disable_dict_factory=True,
                                 one_row=True)
        elif tables == "partie":
            new_id = basic_query("SELECT MAX(idPartie) FROM partie", [], disable_dict_factory=True, one_row=True)
        elif tables == "parametre":
            new_id = basic_query("SELECT MAX(id) FROM parametre", [], disable_dict_factory=True, one_row=True)
        elif tables == "dictionnaire":
            new_id = basic_query("SELECT MAX(idMot) FROM dictionnaire", [], disable_dict_factory=True, one_row=True)
        else:
            raise Exception("Table inconnue")

        if new_id[0] is None:
            return 1
        else:
            return new_id[0] + 1
    except sqlite3.Error as error:
        generationIdMaxError(str(error))


# Convention uc = unencrypted, ec = encrypted
def is_valid_inscription(pseudo: str, uc_password: str):
    querry = basic_query("SELECT count(*)=0 FROM utilisateur WHERE pseudo =?", (pseudo,),
                         one_row=True, disable_dict_factory=True)
    pseudo_available = querry[0] == 1 and pseudo != None and pseudo != ""
    is_valid_password, message = fonctions.is_valid_password(uc_password)
    return pseudo_available and is_valid_password, message


def save_inscription(pseudo: str, uc_password: str) -> None:
    ec_password = fonctions.encrypt(uc_password)
    id_joueur = generate_max_id("utilisateur")
    basic_insert("INSERT INTO utilisateur (idUtilisateur,pseudo,password,scoreUtilisateur) VALUES (?,?,?,?)",
                 (id_joueur, str(pseudo), str(ec_password), 0))


def is_good_password(pseudo: str, uc_password: str):  # -> bool,string:
    querry = basic_query("SELECT password FROM utilisateur WHERE pseudo = ?", (pseudo,), True, True)
    if querry == [] or querry == None:
        return False, "Joueur non répertorié dans la base de donnée"
    elif fonctions.encrypt(uc_password) == querry[0]:
        return True, "Bon identifiant"
    else:
        return False, "Mauvais identifiant"


def connect(pseudo: str):
    querry = basic_query("SELECT * FROM utilisateur WHERE pseudo = ?", (pseudo,), one_row=True)
    return (querry["idUtilisateur"],
            querry["pseudo"],
            querry["parametreDernierePartie"],
            querry["partieEnCours"])  # renvoie pseudo, paramètre derniere partie et partie en cours


def register_game(motATrouver: string, idParam: int, idJoueur: int = -1) -> int:
    date = basic_query("SELECT CURRENT_TIMESTAMP", [], True, True)[0]
    idPartie = generate_max_id("partie")
    querry = "INSERT INTO partie (idPartie, parametre,estEnCours,idJoueur,motATrouver,date,tourActuel,aGagne,scorePartie) " \
             "VALUES (?,?,?,?,?,?,?,?,?)"
    param_sql = (idPartie, idParam, 1, idJoueur, motATrouver, date, 0, 0, 0)
    basic_insert(querry, param_sql)
    return idPartie


def get_id_param(nb_essais: int, nb_lettres: int, difficulté: int) -> int:
    idParam = basic_query("SELECT id FROM parametre WHERE longueur = ? AND nbEssais = ? AND difficulte = ?",
                          (nb_lettres, nb_essais, difficulté), True, True)
    if idParam == [] or idParam == None:
        # Creer param
        idParam = generate_max_id("parametre")
        basic_insert("INSERT INTO parametre (id,longueur,nbEssais,difficulte) VALUES (?,?,?,?)",
                     (idParam, nb_lettres, nb_essais, difficulté))
    else:
        idParam = idParam[0]
    return idParam


def update_current_game_utilisateur(idUtilisateur: int, idPartie: int) -> None:
    """
    Met à jour la partie en cours d'un utilisateur
    :param idUtilisateur
    :param idPartie
    """
    basic_query("UPDATE utilisateur SET partieEnCours = ? WHERE idUtilisateur = ?",
                (idPartie, idUtilisateur), commit=True)


# Calcul des scores
def calcul_score_partie(idPartie: int) -> None:
    idParam = basic_query("SELECT parametre FROM partie WHERE idPartie = ?", (idPartie,), disable_dict_factory=True,
                          one_row=True)[0]

    L, E, D = basic_query("SELECT longueur,nbEssais,difficulte FROM parametre WHERE id =?", (idParam,),
                          disable_dict_factory=True, one_row=True)

    T = basic_query("SELECT count(numLigne) FROM tentative WHERE idPartie = ?", (idPartie,),
                    disable_dict_factory=True, one_row=True)
    scorePartie = D * (L + 13 - E + T[0] // 2)

    basic_query("UPDATE partie SET scorePartie = ? WHERE idPartie = ?", (scorePartie,idPartie,), commit=True)
    return scorePartie


def calcul_score_utilisateur(idUtilisateur: int) -> None:  # refresh le score de l'utilisateur
    scorePartieList = basic_query("SELECT scorePartie FROM partie WHERE idJoueur = ?", (idUtilisateur,),
                                  disable_dict_factory=True)
    for i in range(len(scorePartieList)):
        scorePartieList[i] = scorePartieList[i][0]
    newScoreUtilisateur = sum(scorePartieList)
    basic_query("UPDATE utilisateur SET scoreUtilisateur = ? WHERE idUtilisateur=?",
                (newScoreUtilisateur, idUtilisateur), commit=True)


def add_score_utilisateur(idPartie: int) -> None:
    idJoueur = basic_query("SELECT idJoueur FROM partie WHERE idPartie = ?", (idPartie,), disable_dict_factory=True,
                           one_row=True)[0]

    scoreUtilisateur = basic_query("SELECT scoreUtilisateur FROM utilisateur WHERE idUtilisateur = ?", (idJoueur,),
                                   disable_dict_factory=True, one_row=True)[0]
    scorePartie = basic_query("SELECT scorePartie FROM partie WHERE idPartie = ?", (idPartie,),
                              disable_dict_factory=True, one_row=True)[0]
    newScoreUtilisateur = scoreUtilisateur + scorePartie

    basic_query("UPDATE utilisateur SET scoreUtilisateur = ? WHERE idUtilisateur = ?", (newScoreUtilisateur,idJoueur,), commit=True)


# gets
def get_leaderboard_list() -> list:
    leaderboardList = basic_query("SELECT pseudo,scoreUtilisateur FROM utilisateur", (), disable_dict_factory=True)
    fonctions.tri_fusion(leaderboardList)
    return leaderboardList


def get_rang(idUtilisateur: int) -> int:
    Pseudo, scoreUtilisateur = basic_query("SELECT pseudo,scoreUtilisateur FROM utilisateur WHERE idUtilisateur = ?",
                                           (idUtilisateur,), disable_dict_factory=True, one_row=True)
    Leaderboard = get_leaderboard_list()
    rang = fonctions.index_of_custom(Leaderboard, (Pseudo, scoreUtilisateur))
    return rang


def get_nb_parties_jouees(idUtilisateur: int) -> int:
    nbPartiesJouees = basic_query("SELECT count(estEnCours) FROM partie "
                                  "WHERE idJoueur = ? AND estEnCours = ?",
                                  (idUtilisateur, 0), disable_dict_factory=True)[0][0]  # format [(number,)]
    return nbPartiesJouees


def get_nb_parties_gagnees(idUtilisateur: int) -> int:
    nbPartiesGagnees = basic_query("SELECT count(aGagne) FROM partie WHERE idJoueur = ? AND aGagne = ?",
                                   (idUtilisateur, True), disable_dict_factory=True)[0][0]  # format [(number,)]
    return nbPartiesGagnees


def get_longueur_preferee(idUtilisateur: int) -> int:
    longueurList = basic_query("SELECT parametre.longueur FROM partie "
                               "JOIN parametre ON partie.parametre = parametre.id "
                               "WHERE partie.idJoueur = ?",
                               (idUtilisateur,), disable_dict_factory=True)
    for i in range(len(longueurList)):
        longueurList[i] = longueurList[i][0]
    return fonctions.most_common_in_list(longueurList, "longueur")


def get_difficulte_preferee(idUtilisateur: int) -> int:
    difficulteList = basic_query("SELECT parametre.difficulte FROM partie "
                                 "JOIN parametre ON partie.parametre = parametre.id"
                                 " WHERE partie.idJoueur = ?",
                                 (idUtilisateur,), disable_dict_factory=True)
    for i in range(len(difficulteList)):
        difficulteList[i] = difficulteList[i][0]
    return fonctions.most_common_in_list(difficulteList, "difficulty")


def get_statistiques(idUtilisateur: int) -> list:
    scoreUtilisateur = basic_query("SELECT scoreUtilisateur FROM utilisateur"
                                   " WHERE idUtilisateur = ?",
                                   (idUtilisateur,), disable_dict_factory=True)[0][0]
    rang = get_rang(idUtilisateur)
    nbPartieJouees = get_nb_parties_jouees(idUtilisateur)
    nbPartieGagnees = get_nb_parties_gagnees(idUtilisateur)

    tauxDeVictoire = int(100 * nbPartieGagnees)
    if nbPartieJouees != 0:
        tauxDeVictoire = int(100 * nbPartieGagnees / nbPartieJouees)

    longueurPreferee = get_longueur_preferee(idUtilisateur)
    difficultePreferee = get_difficulte_preferee(idUtilisateur)

    return [rang, scoreUtilisateur, nbPartieJouees, nbPartieGagnees, tauxDeVictoire, longueurPreferee,
            difficultePreferee]


def get_historique_parties(idUtilisateur: int) -> list:
    parties = basic_query(
        "SELECT * FROM partie WHERE idJoueur = ?",
        (idUtilisateur,))
    lignes = []
    for i in range(len(parties)):
        partie = parties[i]
        param = basic_query("SELECT * FROM parametre WHERE id = ?", (partie["parametre"],), one_row=True)
        nbTentative = basic_query("SELECT count(*) FROM tentative WHERE idPartie = ?", (partie["idPartie"],),
                                  one_row=True, disable_dict_factory=True)[0]
        etatPartie = ""
        if partie["estEnCours"] == 0 and partie["aGagne"] == 1:
            etatPartie = "Gagnée"
        elif partie["estEnCours"] == 0 and partie["aGagne"] == 0:
            etatPartie = "Perdue"
        scorePartie = partie["scorePartie"]
        if scorePartie is None:
            scorePartie = " - "
        if partie["estEnCours"] == 1:
            scorePartie = " - "
            etatPartie = "En cours"
            partie["motATrouver"] = "🥸"
        partie_w_detail = [partie["idPartie"], str(nbTentative) + "/" + str(param["nbEssais"]), param["longueur"],
                           partie["motATrouver"], etatPartie, scorePartie, param["difficulte"], partie["date"]]
        lignes.append(partie_w_detail)
    return lignes


def delete_partie_by_id(idPartie: int) -> None:
    """
    Supprime une partie et les tentatives associées de la base de données à partir de son id
    :param idPartie:
    """
    basic_query("DELETE FROM tentative WHERE idPartie = ?", (idPartie,), commit=True)
    basic_query("DELETE FROM partie WHERE idPartie = ?", (idPartie,), commit=True)


def save_tentative(idPartie, answer):
    """
    Sauvegarde une tentative à partir de l'id de la partie et du mot entré par l'utilisateur
    :param idPartie:
    :param answer:
    :return: Numéro de tentative
    """
    num_ligne = basic_query("SELECT count(*)+1 FROM tentative WHERE idPartie=?",
                            (idPartie,), disable_dict_factory=True, one_row=True)
    basic_insert("INSERT INTO tentative (idPartie, numLigne, mot) VALUES (?, ?, ?)", (idPartie, num_ligne[0], answer))
    return num_ligne[0]
