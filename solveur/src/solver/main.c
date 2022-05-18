#include "../io/interface.h"
#include "../dict/dict.h"
#include <stdio.h>


#define IO_TYPE "console"

int main() {
    
    print_hello();
    
    dict_t* dictionnaires[15];

    int code_play_or_quit[2] = { 0, 1};

    bool playing  = true;
    while (playing)
    {
        print_begining_msg();

        int input = ask_code(code_play_or_quit);

        if (input == 1)
        {
            playing = false;
            break;
        }

        int world_length = ask_for_word_length();

        if (dictionnaires[world_length] == NULL)
        {
            //load
        }
        else 
        {
            //récupère
            //range dans dictionnaires
        }

        int exit_code = game_loop(world_length);//, dico)

        switch (exit_code)
        {
        case 0:
            congratulations();
            break;
        case 99:
            playing = false;
            continue;
            break;
        default:
            break;
        };

    }

    //goodbye message
    

    return 0;
}


int game_loop( int longueur)//, struct word_list candidate_words)
{
    bool hasWon = false;
    bool wantToQuit = false;
    //list / tableau mot candidat 
    while (!wantToQuit && !hasWon)
    {
        //récupère mot candidat à partir de mot candidat précédent + nouvelle info

        //calcul meilleur candidat, le renvoie

        //récupère réponse, ajoute info
        //if info == code exit, return exit code
    }

    if (wantToQuit) { return 99;}
    if (hasWon) {return 0;}
    // eventuellement, msg erreur
}


