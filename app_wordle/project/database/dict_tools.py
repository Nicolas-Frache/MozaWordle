from project.database.db_tools import basic_query
from random import randrange

from project.exceptions import badParamGetRandomWordError, invalidDifficultyGetRandomWordError


def est_dans_dict(mot: str):
    """
    Vérifie si un mot est dans le dictionnaire
    """
    x = basic_query("Select count(*)=1 from dictionnaire where mot = ?",
                    (mot,),
                    one_row=True,
                    disable_dict_factory=True)
    return x[0] == 1


# Renvoie un mot aléatoire
def get_random_word(longueur: int, difficulte: int):
    try:
        words = basic_query("SELECT mot FROM dictionnaire WHERE longueur = ? ORDER BY frequence",
                            (longueur,), disable_dict_factory=True)

        if difficulte == 3:
            return words[randrange(0, len(words) // 3)][0]
        elif difficulte == 2:
            return words[randrange(len(words) // 3, 2 * len(words)) // 3][0]
        elif difficulte == 1:
            return words[randrange(2 * len(words) // 3, len(words))][0]
        invalidDifficultyGetRandomWordError()
    except Exception as e:
        badParamGetRandomWordError(str(e))
