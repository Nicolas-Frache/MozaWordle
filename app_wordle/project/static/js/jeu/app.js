import clavier from './clavier.js';
import grille from './grille.js';
import server_requester from './server_requester.js';

window.onload = function () {
    // 1 - Constantes
    const table = document.getElementById('table');
    const nb_essais = table.rows.length;
    const nb_lettres = table.rows[0].cells.length;
    const solution = document.getElementById('solution').innerText.toUpperCase();
    const char = ['A', 'Z', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 'Q', 'S', 'D', 'F', 'G',
        'H', 'J', 'K', 'L', 'M', 'W', 'X', 'C', 'V', 'B', 'N', 'BACKSPACE', 'ENTER'];

    // 2 - HANDLERS
    for (let i in char) { // Ajouts d'evenements lors du clic sur le clavier virtuel
        let touche = document.getElementById(char[i]);
        touche.addEventListener('mousedown', () => {clavier.press_key(char[i]);})
        touche.addEventListener('mouseup', () => {clavier.release_key(char[i]);})
    }
    document.addEventListener("keydown", (e) => {
        if (char.includes(e.key.toUpperCase())) { // S'il s'agit d'une touche valide
            clavier.press_key(e.key.toUpperCase());
        }
    });
    document.addEventListener("keyup", (e) => {
        if (char.includes(e.key.toUpperCase())) { // S'il s'agit d'une touche valide
            clavier.release_key(e.key.toUpperCase());
        }
    });


    // 3 - Initialisation grille
    grille.init(nb_essais, nb_lettres, solution);
    server_requester.est_dans_dictionnaire("voiture");
}
