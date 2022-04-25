import grille from './grille.js';

/**
 * A l'enfoncement d'une touche = changement visuel
 */
const press_key = (value) => {
    // Declenchee lors d'un clic sur le clavier ou sur une touche du clavier
    if (grille.isPlaying()) {
        document.getElementById(value.toUpperCase()).style.backgroundColor = '#003F87';
    }
}

/**
 * Au relachement d'une touche = action du clic
 */
const release_key = (value) => {
    if (grille.isPlaying()) {
        document.getElementById(value.toUpperCase()).style.backgroundColor = '#25C9C973';

        switch(value) {
        case
            'ENTER'
        : // Si ENTER
            grille.valider_ligne();
            break;
        case
            'BACKSPACE'
        : // Si BACKSPACE
            grille.supprimer();
            break;
        default: // Si LETTRE
            grille.ecrire(value);
        }
    }
}

export default {
    release_key,
    press_key
}