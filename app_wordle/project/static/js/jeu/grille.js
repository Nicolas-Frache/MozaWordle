// Tableau à plusieurs dimensions contenant les lettres de la grille
let grille,
    ligne_actuelle = 0,
    colonne_actuelle = 0,
    nb_lettres,
    solution;

function init(nb_essais, nb_lettres_param, solution_param) {
    nb_lettres = nb_lettres_param;
    solution = solution_param;

    // Initialisation de la grille vide
    grille = new Array(nb_essais);
    for (let i = 0; i < nb_lettres; i++) {
        grille[i] = new Array(nb_lettres);
    }

    // Il est possible que la grille soit déjà partiellement remplie donc on
    // synchronise avec l'html au cas où
    for (let i = 0; i < nb_essais; i++) {
        for (let j = 0; j < nb_lettres; j++) {
            if (document.getElementById("l" + i + "c" + j).innerHTML !== "") {
                grille[i][j] = document.getElementById("l" + i + "c" + j).innerHTML;
                ligne_actuelle = i + 1;
            }
        }
    }

    // On finit l'initialisation en coloriant toute les lignes (sans tester si les mots sont dans le dictionnaire
    // car cela a déjà été fait quand le joueur a commencé la partie)
    for (let i = 0; i < nb_essais; i++) {
        colorier_ligne(i, solution)
    }
}

/**
 * Ecrit une lettre dans la grille et décaler la colonne actuelle si possible
 */
function ecrire(lettre) {
    document.getElementById('l' + ligne_actuelle + 'c' + colonne_actuelle).innerText = lettre;
    grille[ligne_actuelle][colonne_actuelle] = lettre;
    if (colonne_actuelle < nb_lettres - 1) {
        colonne_actuelle++;
    }
}

/**
 * Supprimer une lettre de la grille et décale la colonne actuelle
 */
function supprimer() {
    if(colonne_actuelle >= 1){
        colonne_actuelle--;
        document.getElementById('l' + ligne_actuelle + 'c' + colonne_actuelle).innerText = "";
        grille[ligne_actuelle][colonne_actuelle] = undefined;
    }
}

function valider_ligne() {
    let victoire = true;
    let valide = true;

    //On vérifie que le mot de ligne actuelle soit de la bonne longueur
    if(mot(ligne_actuelle).length !== solution.length) {
        valide = false;
    }

    //TODO 2 : vérifier si le mot de ligne actuelle est dans le dictionnaire (appel fetch)
    // SI OUI :
    if(valide) {
        colorier_ligne(ligne_actuelle, true);
        ligne_actuelle++;
        colonne_actuelle = 0;
    }

    // Test de la victoire
    for (let i = 0; i < nb_lettres; i++) {
        if (grille[ligne_actuelle-1][i] !== solution[i]) {
            victoire = false;
        }
    }
    if (victoire) {
        //TODO : Annonce de la victoire
        console.log("Victoire");
    } else {

    }
}

/**
 * Renvoie le resultat en comparant la proposition à la solution
 * Le résultat est un array
 * 0 → lettre non présente
 * 1 → lettre présente, mais pas à la bonne place
 * 2 → lettre à la bonne place
 */
function resultat (proposition, solution) {
    // Precondition : proposition et solution sont de meme longueur
    let result = new Array(solution.length);
    for (let i = 0; i< result.length; i++) { result[i] = 0; }
    let tested = new Array(solution.length);
    for (let i = 0; i< tested.length; i++) { tested[i] = false; }

    // Recherche des carracteres à la bonne place
    for (let i = 0; i<proposition.length; i++) {
        if (proposition[i] === solution[i]) {
            result[i] = 2;
            tested[i] = true;
        }
    }

    for (let i = 0; i<proposition.length; i++) {
        if (result[i] !== 2) {
            for (let j = 0; j<solution.length; j++) {
                if (i!==j && proposition[i] === solution[j] && tested[j] === false) {
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
    if (typeof grille[numLigne][0] !== 'undefined') { // On ne colorie que si la ligne est bien remplie

        let proposition = mot(numLigne);

        // On genere les couleurs en comparant la proposition a la reponse
        let result = resultat(proposition, solution);

        // On colorie les cases
        // TODO : ameliorer le style
        for (let i = 0; i<nb_lettres; i++) {
            switch (result[i]) {
                case 1:
                    document.getElementById('l'+ numLigne +'c'+ i).style.backgroundColor="yellow";
                    break;
                case 2:
                    document.getElementById('l'+ numLigne +'c'+ i).style.backgroundColor="red";
                    break;
            }
        }
    }
}

/**
 * Reconstitue le mot proposé à la ligne numLigne à partir de la grille
 */
function mot(numLigne) {
    let proposition = "";
    for (let i = 0; i < nb_lettres; i++) {
        proposition = proposition + grille[numLigne][i];
    }
    return proposition
}

export default {
    init,
    ecrire,
    supprimer,
    valider_ligne,
}