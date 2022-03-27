import pandas as pd
import unidecode as unidecode


# Retourne un dataframe contenant le dictionnaire filtré selon nos besoins
def get_dict_data():
    print("Début du traitement du dictionnaire...")

    # On importe la base de données lexical du projet Lexique 3
    # Voir https://github.com/chrplr/openlexicon/tree/master/scripts
    lex = pd.read_csv("http://www.lexique.org/databases/Lexique382/Lexique382.tsv", sep='\t')
    taille_initiale = lex.memory_usage().sum()
    nb_mots_initial = lex.size

    # On conserve uniquement les colonnes nécessaires
    lex = lex.filter(items=['Index', 'ortho', 'nblettres', 'freqlivres'])

    # On supprime les lignes avec un nombre de lettres inférieur à 3
    lex = lex.loc[(lex.nblettres >= 3)]

    # Supression des mots composés (ex: "a-capella") ou avec un espace (ex: "a capella")
    caracteres_interdits = ' -_,;.'
    lex = lex[lex['ortho'].str.match(f'^.*[{caracteres_interdits}].*$') == False]

    # Supression des accents avec la librairie unidecode
    lex["ortho"] = lex["ortho"].map(lambda x: unidecode.unidecode(x))

    # suprression des doublons (des mots avec la même orthographe mais provenant de lemmes différents)
    # On commence par trier dans l'odre de fréquence pour garder les mots les plus fréquents
    lex.sort_values(by='freqlivres', na_position='last', inplace=True, ascending=False)
    # Puis on supprime les doublons en gardant le premier
    lex.drop_duplicates(subset=['ortho'], keep='first', inplace=True)

    # Enfin on reinitialise les index
    lex.reset_index(drop=True, inplace=True)

    print(f'- Nombre de mots conservés : {lex.size}')
    print(f'- Nombre de mots filtrés durant le traitement : {nb_mots_initial - lex.size}')
    print(
        f'- Mémoire économisée par le traitement : {float("{:.2f}".format((taille_initiale - lex.memory_usage().sum()) / 1024 / 1024))} Mo,')
    print(f'    - soit {float("{:.1f}".format(100 - (lex.memory_usage().sum() * 100 / taille_initiale)))}% d\'économie')

    # On renomme les colonnes pour correspondre à notre schema de base de données
    lex.rename(columns={'ortho': 'mot', 'freqlivres': 'frequence', 'nblettres': 'longueur'}, inplace=True)

    # On exporte le dictionnaire
    return lex
