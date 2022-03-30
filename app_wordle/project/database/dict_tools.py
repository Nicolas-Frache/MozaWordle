from db_tools import basic_query, get_db


def est_dans_dict(mot: str):
    """
    Vérifie si un mot est dans le dictionnaire
    """
    x = basic_query("Select count(*)=1 from dictionnaire where mot = ?",
                    (mot,),
                    disable_dict_factory=True)
    return x[0][0] == 1
