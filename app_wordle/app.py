from crypt import methods
import traceback
from flask import Flask, render_template, redirect, request,session
from flask_session import Session

from project.database import db_tools, dict_tools

# Création de l'instance de l'application Flask et définition
# du chemin du dossier contenant les templates et les fichiers statics
app = Flask(__name__, template_folder='project/templates', static_folder='project/static')
app.config["SESSION_PERMANENT"] = False
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


@app.route('/')
@app.route('/home')
@app.route('/menu')
def home():
    return render_template("pages/Menu.html")


@app.route('/jeu')
def jeu():
    return render_template("pages/Jeu.html", nb_essais=6, nb_lettres=7)


@app.route('/regles')
def regles():
    return render_template("pages/Regles.html")


@app.route('/leaderboard')
def leaderboard():
    return render_template("pages/Leaderboard.html")


@app.route('/test')
def test():
    try:
        print(dict_tools.est_dans_dict("voiture"))
    except Exception as e:
        return handle_error(e)
    return 'test'


@app.route('/login', methods=["GET","POST"])
def loginPage():
    if request.method == "POST":
        pseudo, password = request.form.get("pseudo"),request.form.get("password")
        passwordVerification = db_tools.GoodPassword(pseudo,password)
        if passwordVerification[0] and (("idJoueur" in session and session["idJoueur"] == None) or not "idJoueur" in session):
            session["idJoueur"],session["pseudo"],session["paramLastGame"],session["currentGame"] = db_tools.Connect(pseudo)
            return redirect("/")
        else :
            #TODO repport sur la page login avec un message d'erreur
            return handle_error("Mauvais identificateur") # Temporaire
    else :
        return render_template("pages/Login.html")

@app.route('inscription', methods=["POST"])
def Inscription():
    if request.method == "POST":
        #Handle inscription
        pseudo, password = request.form.get("pseudo"),request.form.get("password")

        if (db_tools.Valid_Inscription(pseudo,password)):
            Inscription(pseudo,password)
            return redirect("/")
        else :
            return handle_error("Inscription non valide") #Ajouter message d'erreur custom si pseudo déjà pris / mauvais password

    else :
        return handle_error("Accès à la page d'inscription avec requête GET")


@app.route('/logout')
def logout():
    session["idJoueur"] = None
    session["pseudo"] = None 
    session["paramLastGame"] = None
    session["currentGame"] = None
    return redirect("/")


@app.cli.command('initdb')
def initdb_command():
    # Initialisation de la base de données
    db_tools.create_db()


# Pour l'execution en ligne de commande directement avec 'Python3 app.py'
if __name__ == '__main__':
    app.run(debug=1)
