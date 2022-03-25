from flask import Flask, render_template

# Création de l'instance de l'application Flask et définition
# du chemin du dossier contenant les templates et les fichiers statics
app = Flask(__name__, template_folder='project/templates', static_folder='project/static')


@app.route('/')
@app.route('/home')
def hello_world():
    return render_template("pages/home.html")


@app.route('/test')
def test():
    return 'test'


# Pour l'execution en ligne de commande directement avec 'Python3 app.py'
if __name__ == '__main__':
    app.run()
