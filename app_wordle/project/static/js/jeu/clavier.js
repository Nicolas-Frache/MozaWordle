import grille from './grille.js';

/**
 * A l'enfoncement d'une touche = changement visuel
 */
const press_key = (value) => {
    // Declenchee lors d'un clic sur le clavier ou sur une touche du clavier
    document.getElementById(value.toUpperCase()).style.backgroundColor = '#003F87';
}

/**
 * Au relachement d'une touche = action du clic
 */
const release_key = (value) => {
    document.getElementById(value.toUpperCase()).style.backgroundColor = '#25C9C973';

    //TODO : regarder si c'est une lettre ou une des autres touches
    // SI LETTRE :
    grille.ecrire(value);

    // SI ENTER :
    // grille.valider_ligne();

    // SI DELETE :
    // grille.supprimer();
}

export default {
    release_key,
    press_key
}