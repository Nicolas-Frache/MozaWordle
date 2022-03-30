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


# Renvoie un mot aléatoire
def get_random_word(longueur: int, frequencemin=0, frequencemax=20000):
    try:
        words = basic_query("SELECT mot FROM dictionnaire WHERE longueur = ? AND frequence >= ? AND frequence <= ?",
                            (longueur, frequencemin, frequencemax,), disable_dict_factory=True)
        return words[randrange(0, len(words))][0]
    except ValueError:
        print("Aucun mot ne correspond aux critères de recherche")
