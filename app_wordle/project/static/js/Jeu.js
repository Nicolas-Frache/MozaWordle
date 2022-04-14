window.onload = function () {
    const table = document.getElementById('table');
    const nb_essais = table.rows.length;
    const nb_lettres = table.rows[0].cells.length;
    const char = ['A','Z','E','R','T','Y','U','I','O','P','Q','S','D','F','G','H','J','K','L','M','W','X','C','V','B','N'];
    let touches = [];

    for (let i in char) { // Ajouts d'evenements lors du clic sur le clavier virtuel
        touches[i] = document.getElementById(char[i]);
        touches[i].addEventListener('mousedown', () => {
            press_key(char[i]);
        })
        touches[i].addEventListener('mouseup', () => {
            release_key(char[i]);
        })
    }

    document.getElementById('BACKSPACE').addEventListener('mousedown', () => {
        press_key('Backspace');
        backspace();
    })
    document.getElementById('BACKSPACE').addEventListener('mouseup', () => {
        release_key('Backspace');
    })
    document.getElementById('ENTER').addEventListener('mousedown', () => {
        press_key('Enter');
        enter();
    })
    document.getElementById('ENTER').addEventListener('mouseup', () => {
        release_key('Enter');
    })

    document.addEventListener("keydown", (e) => {
        if (e.keyCode >= 65 && e.keyCode <= 90){ // S'il s'agit d'une lettre sur le clavier
            press_key(e.key);
        }
        if (e.code === 'Backspace') {
            press_key('Backspace');
            backspace();
        }
        if (e.code === 'Enter') {
            press_key('Enter');
            enter();
        }
    });

    document.addEventListener("keyup", (e) => {
        if (e.keyCode >= 65 && e.keyCode <= 90) { // S'il s'agit d'une lettre sur le clavier
            release_key(e.key);
        }
        if (e.code === 'Backspace' || e.code === 'Enter') {
            release_key(e.code);
        }
    });

    function press_key(value) {
        // Declenchee lors d'un clic sur le clavier ou sur une touche du clavier
        document.getElementById(value.toUpperCase()).style.backgroundColor='#003F87';
    }

    function release_key(value) {
        // Declenchee lorsque la touche est relâchée
        document.getElementById(value.toUpperCase()).style.backgroundColor='#25C9C973';
    }

    function backspace() {
        // Delenchee lors d'un appui sur la touche effacer (backspace)
        console.log('Backspace');
    }

    function enter() {
        // Declenchee lors d'un appui sur la touche enter
        console.log("Enter");
    }

    function write(tentative, lettre, toWrite) {
        document.getElementById('l'+tentative+'c'+lettre).innerText=toWrite;
        document.getElementById('l'+tentative+'c'+lettre).style.backgroundColor="yellow";
        // Temporaire
    }
}
