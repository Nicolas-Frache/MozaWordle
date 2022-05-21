#include "../io/interface.h"
#include <stdio.h>


#ifdef SNOW_ENABLED

#include "../test/snow.h"
#include "../test/test_test.c"
#include "../test/test_attempts_tools.c"
#include "../test/test_dict.c"
snow_main();

#else

int game_loop(int length);

int main() {

    print_hello();
    //int code_play_or_quit[2] = {0, 1};

    bool playing = false;

    while (playing) {
        // TODO - importer le dico
    }
    return 0;
}

#endif
/*

int game_loop(int longueur) {
    //list / tableau mot candidat
    bool stop;
    while (!stop) {
        //récupère mot candidat à partir de mot candidat précédent + nouvelle info

        //calcul meilleur candidat, le renvoie

        //récupère réponse, ajoute info
        //if info == code exit, return exit code
    }

    // eventuellement, msg erreur
    return 0;
}
*/


