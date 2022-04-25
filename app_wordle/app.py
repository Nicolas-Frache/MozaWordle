# from crypt import methods
import traceback
from flask import Flask, render_template, redirect, request, session, jsonify
from flask_session import Session

from project.database.db_tools import save_inscription, register_game, basic_query, delete_partie_by_id, basic_insert
from project.database import db_tools, dict_tools

# Création de l'instance de l'application Flask et définition
# du chemin du dossier contenant les templates et les fichiers statics
from project.database.dict_tools import get_random_word
from project.exceptions import invalidInscription

app = Flask(__name__, template_folder='project/templates', static_folder='project/static')
app.config["SESSION_PERMANENT"] = False
app.secret_key = 'app secret key'
app.config["SESSION_TYPE"] = "filesystem"
Session(app)


def handle_error(error):
    """
    Fonction permettant de gérer les erreurs
    :param Une erreur, personalisée ou non avec un message
    :return: une page html contenant le message d'erreur
    """
    print(traceback.format_exc())
    if hasattr(error, "value") and len(error.args) > 0:
        message = error.value.args[0]
    else:
        message = str(error)
    return render_template("pages/error.html", error=message)


def is_logged_in():
    """
    Vérifie si l'utilisateur est connecté
    :return: True si l'utilisateur est connecté, False sinon
    """
    return "idJoueur" in session and session["idJoueur"] is not None


@app.route('/', methods=['GET', 'POST'])
@app.route('/home')
@app.route('/menu')
def home():
    try:
        if request.method == "GET":
            """
            si connecté :
                récupère parametre
            sinon
                assignation parametre par défaut    
            """
            if is_logged_in() and session.get("paramLastGame") is not None:
                param = basic_query("SELECT * from parametre where id = ?", (session.get("paramLastGame"),),
                                    one_row=True)
                return render_template("pages/menu.html",
                                       longueur=param["longueur"],
                                       tentative=param["nbEssais"],
                                       difficulte=param["difficulte"])
            return render_template("pages/menu.html", longueur=7, tentative=6, difficulte=2)
        elif request.method == "POST":
            if is_logged_in():
                _nb_essais = request.form.get("tentatives", type=int)
                _nb_lettres = request.form.get("taille", type=int)
                _difficulte = request.form.get("difficulte", type=int)
                # Mise à jour des paramètres de l'utilisateur
                idParam = db_tools.get_id_param(_nb_essais, _nb_lettres, _difficulte)
                session["paramLastGame"] = idParam
                basic_query("UPDATE utilisateur SET parametreDernierePartie = ? WHERE idUtilisateur = ?",
                            (idParam, session.get("idJoueur"),), commit=True)
            return redirect("/jeu")
    except Exception as e:
        return handle_error(e)


@app.route('/jeu')
def jeu():
    try:
        nouvelle_partie = False
        if is_logged_in():
            param = basic_query("SELECT * from parametre where id = ?",
                                (session.get("paramLastGame"),),
                                one_row=True)
            if session.get("currentGame") is not None:
                lastGame = basic_query("SELECT * from partie where idPartie = ?",
                                       (session.get("currentGame"),),
                                       one_row=True)
                if lastGame["estEnCours"] == 1:
                    """
                    Si je le joueur a changé ses paramètres, on supprime la partie en cours dans la bd,
                    (on ne conserve pas de partie non terminée), puis on en crée une nouvelle
                    """
                    if lastGame["parametre"] != session.get("paramLastGame"):
                        # Supression partie
                        delete_partie_by_id(lastGame["idPartie"])
                        nouvelle_partie = True
                    else:
                        """
                        Si le joueur n'a pas changé ses paramètres, on lui permet de continuer la partie
                        en cours en chargeant les tentatives précèdentes
                        """
                        # Récupération des tentatives
                        tentatives = basic_query(
                            "SELECT mot from tentative where idPartie = ? ORDER BY numLigne ASC",
                            (lastGame["idPartie"],))
                        # On "flatten" la liste de tuples pour avoir une simple liste de mot
                        for i in range(0, len(tentatives)):
                            tentatives[i] = list(tentatives[i]["mot"].upper())
                        return render_template("pages/jeu.html",
                                               nb_essais=param["nbEssais"],
                                               nb_lettres=param["longueur"],
                                               mot=lastGame["motATrouver"],
                                               tentatives=tentatives)
                else:
                    nouvelle_partie = True
            else:
                nouvelle_partie = True

            if nouvelle_partie:
                # Création d'une nouvelle partie
                mot = dict_tools.get_random_word(param["longueur"], param["difficulte"])
                # Creation de la partie dans la bd:
                idPartie = register_game(mot, session.get("paramLastGame"), session.get("idJoueur"))
                session["currentGame"] = idPartie

                db_tools.update_current_game_utilisateur(session.get("idJoueur"), idPartie)

                return render_template("pages/jeu.html",
                                       nb_essais=param["nbEssais"],
                                       nb_lettres=param["longueur"],
                                       mot=mot)
        else:
            """
            Si le joueur n'est pas connecté, charge une partie
            avec des paramètres pas défaut
            """
            print("assignation paramètres par défaut")
            return render_template("pages/jeu.html",
                                   nb_essais=6,
                                   nb_lettres=7,
                                   mot=get_random_word(longueur=7, difficulte=1))
        raise Exception("Situation pas encore gérée")
    except Exception as e:
        return handle_error(e)


@app.route('/regles')
def regles():
    return render_template("pages/regles.html")


@app.route('/leaderboard')
def leaderboard():
    return render_template("pages/leaderboard.html")


@app.route('/test')
def test():
    try:
        print(dict_tools.est_dans_dict("voiture"))
    except Exception as e:
        return handle_error(e)
    return 'test'


@app.route('/login', methods=["GET"])
def loginPage_Get():
    return render_template("pages/login.html")


@app.route('/login', methods=["POST"])
def loginPage_Post():
    try:
        pseudo = request.form.get("pseudo")
        password = request.form.get("password")
        passwordVerification = db_tools.is_good_password(pseudo, password)
        if passwordVerification[0] and (
                ("idJoueur" in session and session["idJoueur"] is None) or not "idJoueur" in session):

            session["idJoueur"], session["pseudo"], session["paramLastGame"], session["currentGame"] = db_tools.connect(
                pseudo)
            return redirect("/")
        else:
            # TODO repport sur la page login avec un message d'erreur
            return handle_error("Mauvais identificateur")  # Temporaire
    except Exception as e:
        return handle_error(e)


@app.route('/inscription', methods=["POST"])
def inscription_page():
    try:
        # Handle inscription
        pseudo = request.form.get("pseudo")
        password = request.form.get("password")

        valide,_message = db_tools.is_valid_inscription(pseudo, password)
        if valide:
            save_inscription(pseudo, password)
            session["idJoueur"], session["pseudo"], session["paramLastGame"], session["currentGame"] = db_tools.connect(
                pseudo)
            return redirect("/")
        else:
            return render_template("pages/login.html",message = _message)
    except Exception as e:
        return handle_error(e)


@app.route('/logout')
def logout():
    session["idJoueur"] = None
    session["pseudo"] = None
    session["paramLastGame"] = None
    session["currentGame"] = None
    return redirect("/")


@app.route('/api/estValideMot/<mot>', methods=["GET"])
def estValideMot(mot):
    """
    Endpoint permettant de vérifier si un mot est dans le dictionnaire et
    également d'enregistrer la tentative dans la base de données
    :param mot à vérifier
    :return: un fichier JSON contenant la réponse
    """
    try:
        valid = dict_tools.est_dans_dict(mot)
        if valid and is_logged_in():
            # Enregistrement tentative
            num_tentative = db_tools.save_tentative(session.get("currentGame"), mot)
            game = basic_query("SELECT * from partie where idPartie = ?", (session.get("currentGame"),), one_row=True)
            basic_insert("UPDATE partie SET tourActuel = ? WHERE idPartie = ?", (num_tentative, game["idPartie"],))
            # Test de la victoire ou defaite
            if game["motATrouver"] == mot:
                # Victoire
                basic_insert("UPDATE partie SET estEnCours = 0 WHERE idPartie = ?", (game["idPartie"],))
                basic_insert("UPDATE partie SET aGagne = 1 WHERE idPartie = ?", (game["idPartie"],))
            else:
                param = basic_query("SELECT * from parametre where id = ?", (session.get("paramLastGame"),),
                                    one_row=True)
                if num_tentative >= param["nbEssais"]:
                    # Défaite
                    basic_insert("UPDATE partie SET estEnCours = 0 WHERE idPartie = ?", (game["idPartie"],))
                    basic_insert("UPDATE partie SET aGagne = 0 WHERE idPartie = ?", (game["idPartie"],))

    except Exception as e:
        return jsonify({
            "status": "error",
            "message": str(e)})
    return jsonify({
        "status": "success",
        "estValide": valid
    })


@app.cli.command('initdb')
def initdb_command():
    # Initialisation de la base de données
    db_tools.create_db()


# Pour l'execution en ligne de commande directement avec 'Python3 app.py'
if __name__ == '__main__':
    app.run(debug=True)
