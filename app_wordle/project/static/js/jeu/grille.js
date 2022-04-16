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
    console.log(nb_lettres);

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
    console.log(nb_lettres);
    console.log('l' + ligne_actuelle + 'c' + colonne_actuelle);
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

function valider_ligne(){
    //TODO 1 : vérifier si le mot de ligne actuelle est de la bonne longueur
    //TODO 2 : vérifier si le mot de ligne actuelle est dans le dictionnaire (appel fetch)
    // SI OUI :
    if(true){
        colorier_ligne(ligne_actuelle, true);
        ligne_actuelle++;
    }
    //TODO 3 : Tester la victoire ou la defaite
}

/**
 * Colorie dans le DOM les cases correspondant à la ligne donnée pour la solution
 */
function colorier_ligne(numLigne) {
    // TODO
}


export default {
    init,
    ecrire,
    supprimer,
    valider_ligne,
}