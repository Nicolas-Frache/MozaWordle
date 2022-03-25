# PPII2 «Projet Pluridisciplinaire d'Informatique Intégrative» 2 (2021-2022)

Olivier Festor <<olivier.festor@telecomnancy.eu>>  
Gérald Oster <<gerald.oster@telecomnancy.eu>>  


## WORDLE 



**Membres du groupe E32** :
- Valentin CHANEL
- Matéo DEMANGEON
- Nicolas FRACHE
- Victor VOISIN

## Description du projet

Création dans un premier temps d'une application web reprenant le principe du jeu wordle (https://www.nytimes.com/games/wordle/index.html). En utilisant la technologie Flask en Python.
Puis dans un second temps, d'un solveur en C qui mettra à contribution des structures de données avancées.

[Voir le sujet détaillé disponible en version PDF](./sujet.pdf)

## Instructions d'installation :

1. Cloner ou télécharger le projet sur son ordinateur.


2. Dans le dossier du projet ```./app_wordle/```, creer un environnement virtuel avec la commande :
   ```python3 -m venv ./venv```.


3. Rentrer dans l'environnement virtuel :
    - Sur linux ou équivalent: ```source ./venv/bin/activate```
    - Sur windows: ```.\venv\Scripts\activate.bat```


4. Installer les dépendances: ```pip install -r requirement.txt```.


5. Initialiser la base de données : ```TODO```.


6. Demarrer l'application. Pour cela deux méthodes :
    - Méthode 1: ```flask run -p 3000```
    - Méthode 2: ```python3 app.py```, cette seconde solution ne permet pas de choisir le port.


7. Enfin pour tester l'application en local, accèder à l'url: http://localhost:3000/.


## Conventions pour les messages de commits

Afin de rendre les messages de commits plus clairs nous allons utiliser une version modifiée et adaptée à nos besoins des règles de nommage décrites dans ce dépot : https://github.com/ahmadawais/Emoji-Log/.

*L'utilisation des emojis est facultative mais apporte plus de lisibilité.*

1. `✨ NEW: MESSAGE`
    > Nouvelle fonctionalité.

2. `🔨 IMPROVE: MESSAGE`
    > Amélioration de code existant ou refactoring.

3. `🪲 FIX: MESSAGE`
    > Correction de bug.

4. `💡 DOC: MESSAGE`
    > Documentation liée au projet (commentaires ou readme.md par exemple).

5. `🤖 TEST:MESSAGE`
    > Lié aux test (sauf ceux de performance).

6. `🐎 PERF: MESSAGE`
    > Tests et mesure de performance, ainsi que les calculs de complexité théorique. 

7. `📚 GDP: MESSAGE`
    > Documents de gestion de projet (à ne pas confondre avec *💡 DOC* pour l'aspect technique).

8. `🔀 MERGE: MESSAGE`
    > Merges de branches.

9. `🔧 CONF: MESSAGE`
    > Fichier de configuration (*requirement.txt* par exemple).

10. `🚧 WIP: MESSAGE`
    > **Work in progress**: Méthode partiellement écrite qu'on rélègue à un autre (utiliser une branche temporaire).

11. `💄 COS: MESSAGE`
    > Changement cosmétique (UI).

12. `📌 RELEASE: MESSAGE`
    > Livrable de fin de sprint. 