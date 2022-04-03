window.onload = function () {
    const table = document.getElementById('table');
    const nb_essais = table.rows.length;
    const nb_lettres = table.rows[0].cells.length;
    let char = ['A','Z','E','R','T','Y','U','I','O','P','Q','S','D','F','G','H','J','K','L','M','W','X','C','V','B','N'];
    let touches = [];

    for (let i in char) {
        touches[i] = document.getElementById(char[i]);
        touches[i].addEventListener('mousedown', () => {
            press_key(char[i]);
        })
        touches[i].addEventListener('mouseup', () => {
            release_key(char[i]);
        })
    }

    document.addEventListener("keydown", (e) => {
        press_key(e.key);
    });

    document.addEventListener("keyup", (e) => {
        release_key(e.key);
    });

    function press_key(value) {
        // Declenchee lors d'un clic sur le clavier ou sur une touche du clavier
        document.getElementById(value.toUpperCase()).style.backgroundColor='#003F87';
    }

    function release_key(value) {
        // Declenchee lorsque la touche est relâchée
        document.getElementById(value.toUpperCase()).style.backgroundColor='#25C9C973';
    }
}
