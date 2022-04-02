window.onload = function () {
    const table = document.getElementById('table');
    const clavier = document.getElementById('clavier');
    const nb_essais = table.rows.length;
    const nb_lettres = table.rows[0].cells.length;

    document.addEventListener("keydown", (e) => {
        press_key(e);
    });

    document.addEventListener("keyup", (e) => {
        release_key(e);
    });

    function press_key(value) {
        // Declenchee lors d'un clic sur le clavier ou sur une touche du clavier
        document.getElementById(value.key.toUpperCase()).style.backgroundColor='#003F87';
    }

    function release_key(value) {
        document.getElementById(value.key.toUpperCase()).style.backgroundColor='#25C9C973';
    }
}
