from flask import Flask, render_template
from project.database import db_tools

# Création de l'instance de l'application Flask et définition
# du chemin du dossier contenant les templates et les fichiers statics
app = Flask(__name__, template_folder='project/templates', static_folder='project/static')


@app.route('/')
@app.route('/home')
@app.route('/menu')
def hello_world():
    return render_template("pages/Menu.html")

@app.route('/jeu')
def jeu():
    return render_template("pages/Jeu.html")

@app.route('/regles')
def regles():
    return render_template("pages/Regles.html")

@app.route('/leaderboard')
def leaderboard():
    return render_template("pages/Leaderboard.html")

@app.route('/test')
def test():
    return 'test'


@app.route('/login')
def loginPage():
    return render_template("pages/login.html")


@app.cli.command('initdb')
def initdb_command():
    # Initialisation de la base de données
    db_tools.create_db()
    print('Initialized the database.')


# Pour l'execution en ligne de commande directement avec 'Python3 app.py'
if __name__ == '__main__':
    app.run()
