// Tableau à plusieurs dimensions contenant les lettres de la grille
import server_requester from './server_requester.js';


let grille,
    ligne_actuelle = 0,
    colonne_actuelle = 0,
    nb_lettres,
    solution,
    nb_essais,
    mot_complet = false, // true si le mot de la ligne actuelle comporte assez de lettres, false sinon
    idTimeout,
    playing = true;

function init(nb_essais_param, nb_lettres_param, solution_param) {
    nb_lettres = nb_lettres_param;
    solution = solution_param;
    nb_essais = nb_essais_param;

    // Initialisation de la grille vide
    grille = new Array(nb_essais_param);
    for (let i = 0; i < nb_essais_param; i++) {
        grille[i] = new Array(nb_lettres);
    }

    // Il est possible que la grille soit déjà partiellement remplie donc on
    // synchronise avec l'html au cas où
    for (let i = 0; i < nb_essais_param; i++) {
        for (let j = 0; j < nb_lettres; j++) {
            if (document.getElementById("l" + i + "c" + j).innerHTML !== "") {
                grille[i][j] = document.getElementById("l" + i + "c" + j).innerHTML;
                ligne_actuelle = i + 1;
            }
        }
    }

    // On finit l'initialisation en coloriant toutes les lignes (sans tester si les mots sont dans le dictionnaire
    // car cela a déjà été fait quand le joueur a commencé la partie)
    for (let i = 0; i < nb_essais_param; i++) {
        colorier_ligne(i)
    }
}

/**
 * Ecrit une lettre dans la grille et décaler la colonne actuelle si possible
 */
function ecrire(lettre) {
    document.getElementById('l' + ligne_actuelle + 'c' + colonne_actuelle).innerText = lettre;
    grille[ligne_actuelle][colonne_actuelle] = lettre;
    if (colonne_actuelle === nb_lettres - 1) { // Si on se trouve à la dernière colonne
        mot_complet = true;
    }
    if (colonne_actuelle < nb_lettres - 1) {
        colonne_actuelle++;
    }
}

/**
 * Supprimer une lettre de la grille et décale la colonne actuelle
 */
function supprimer() {
    if (colonne_actuelle >= 1 && !mot_complet) {
        colonne_actuelle--;
        document.getElementById('l' + ligne_actuelle + 'c' + colonne_actuelle).innerText = "";
        grille[ligne_actuelle][colonne_actuelle] = undefined;
    }
    if (mot_complet) {
        document.getElementById('l' + ligne_actuelle + 'c' + colonne_actuelle).innerText = "";
        grille[ligne_actuelle][colonne_actuelle + 1] = undefined;
    }
    mot_complet = false;
}

function valider_ligne() {
    let mot = getMot(ligne_actuelle);
    if (!mot_complet) {
        display_message("Votre mot n'est pas complet.")
        return;
    }

    // on vérifie si le mot de ligne actuelle est dans le dictionnaire (appel fetch)
    server_requester.est_dans_dictionnaire(mot.toLowerCase())
        .then(response => {
            // si la requête a réussi :

            if (response === true) {
                // SI VALIDE
                colorier_ligne(ligne_actuelle);
                if (mot === solution) {
                    victoire();
                } // Test de la victoire
                if (mot !== solution && ligne_actuelle + 1 >= nb_essais) {
                    defaite();
                } // Test de la defaite

                ligne_actuelle++;
                mot_complet = false;
                colonne_actuelle = 0;
            } else {
                // SI NON VALIDE
                display_message("Votre mot n'est pas dans le dictionnaire.");
                let grilleDom = document.getElementById("grille_jeu");

                grilleDom.className = "grille_jeu"
                // Utilisation d'un délai pour que le navigateur ait le temps de comprendre
                // ce qu'il se passe et de lancer l'animation de tremblement
                setTimeout(function () {
                    grilleDom.className += " animation_shake"
                }, 10);
            }
        })
        .catch(error => {
            // si la requête a échoué :
            display_message("Une erreur est survenue lors de la requête vers le serveur ▶️ " + error, 20000);
        });
}

/**
 * Renvoie le resultat en comparant la proposition à la solution
 * Le résultat est un array
 * 0 → lettre non présente
 * 1 → lettre présente, mais pas à la bonne place
 * 2 → lettre à la bonne place
 */
function resultat(proposition, solution) {
    // Precondition : proposition et solution sont de meme longueur
    let result = new Array(solution.length);
    for (let i = 0; i < result.length; i++) {
        result[i] = 0;
    }
    let tested = new Array(solution.length);
    for (let i = 0; i < tested.length; i++) {
        tested[i] = false;
    }

    // Recherche des carracteres à la bonne place
    for (let i = 0; i < proposition.length; i++) {
        if (proposition[i] === solution[i]) {
            result[i] = 2;
            tested[i] = true;
        }
    }

    for (let i = 0; i < proposition.length; i++) {
        if (result[i] !== 2) {
            for (let j = 0; j < solution.length; j++) {
                if (i !== j && proposition[i] === solution[j] && !tested[j]) {
                    result[i] = 1;
                    tested[j] = true;
                    break;
                }
            }
        }
    }
    return result;
}

/**
 * Colorie dans le DOM les cases correspondant à la ligne donnée pour la solution
 */
function colorier_ligne(numLigne) {
    let case_grille, touche;
    let grey = "rgb(121, 121, 121)";
    let yellow = "rgb(237, 176, 44)";
    let red = "rgb(182, 0, 0)";
    if (typeof grille[numLigne][0] !== 'undefined') { // On ne colorie que si la ligne est bien remplie
        let proposition = getMot(numLigne);
        // On genere les couleurs en comparant la proposition a la reponse
        let result = resultat(proposition, solution);
        // On colorie les cases
        // TODO : ameliorer le style
        for (let i = 0; i < nb_lettres; i++) {
            case_grille = document.getElementById('l' + numLigne + 'c' + i);
            touche = document.getElementById(proposition[i]);
            switch (result[i]) {
                case 0:
                    if (touche.style.backgroundColor !== red && touche.style.backgroundColor !== yellow) {
                        touche.style.backgroundColor = grey;
                    }
                    break;
                case 1:
                    case_grille.style.backgroundColor = yellow;
                    if (touche.style.backgroundColor !== red) {
                        touche.style.backgroundColor = yellow;
                    }
                    break;
                case 2:
                    case_grille.style.backgroundColor = red;
                    touche.style.backgroundColor = red;
                    break;
            }
        }
    }
}

/**
 * Reconstitue le mot proposé à la ligne numLigne à partir de la grille
 */
function getMot(numLigne) {
    let proposition = "";
    for (let i = 0; i < nb_lettres; i++) {
        proposition = proposition + grille[numLigne][i];
    }
    return proposition
}

function victoire() {
    playing = false;
    server_requester.get_score_fin_partie()
        .then(response => {
            document.getElementById("score").innerText = "Votre score : " + response;
        })
        .catch(error => {
            // si la requête a échoué :
            display_message("Erreur lors de la récupération du score ▶️ " + error, 20000);
        });
    document.getElementById("motV").innerText = "Vous avez trouvé le mot " + solution + ".";
    document.getElementById("end-container").style.display = "block";
    document.getElementById("victoire").style.display = "block";
    console.log("Victoire");
}

function defaite() {
    playing = false;
    document.getElementById("motD").innerText = "Le mot à trouver était " + solution + ".";
    document.getElementById("end-container").style.display = "block";
    document.getElementById("defaite").style.display = "block";
    console.log("Défaite");
}

function display_message(message, time = 2000) {
    // TODO : Améliorer le style (animation par exemple)
    if (typeof idTimeout !== undefined) {
        clearTimeout(idTimeout);
    }

    document.getElementById("message").innerText = message;
    document.getElementById("message").style.display = "block";
    idTimeout = setTimeout(() => document.getElementById("message").style.display = "none", time);
}

function isPlaying() {
    return playing;
}

export default {
    init,
    ecrire,
    supprimer,
    valider_ligne,
    isPlaying,
}