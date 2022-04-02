window.onload = function (){
    // the code to be called when the dom has loaded
    // #document has its nodes
    const table = document.getElementById('table');
    const nb_essais = table.rows.length;
    const nb_lettres = table.rows[0].cells.length;
    jeu(nb_essais, nb_lettres);
}

function jeu(nb_essais, nb_lettres){
    console.log(nb_essais);
    console.log(nb_lettres);
}
