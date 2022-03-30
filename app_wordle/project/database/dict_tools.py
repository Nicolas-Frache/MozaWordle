from database.db_tools import basic_query, get_db
from random import randrange


def est_dans_dict(mot: str):
    """
    Vérifie si un mot est dans le dictionnaire
    """
    x = basic_query("Select count(*)=1 from dictionnaire where mot = ?",
                    (mot,),
                    disable_dict_factory=True)
    return x[0][0] == 1


def get_random_word(longueur: int, frequencemax=1000):
    # Renvoie un mot aléatoire

    words = basic_query("SELECT mot FROM dictionnaire WHERE longueur = ?",
                        (longueur,), disable_dict_factory=True)

    index = randrange(0, len(words))

    return words[index][0]
