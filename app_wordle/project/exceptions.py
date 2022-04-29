def connexionBaseDeDonneeError(previousError=""):
    raise Exception(previousError + " ➡️ Erreur lors de la connexion à la base de données", "E001")


def requetageBaseDeDonneeError(previousError=""):
    raise Exception(previousError + " ➡️ Erreur lors du requêtage de la base de données", "E002")


def badParamGetRandomWordError(previousError=""):
    raise Exception(previousError + ' ➡️ Erreur lors de la récupération du mot aléatoire', "E003")


def invalidDifficultyGetRandomWordError(previousError=""):
    raise Exception(previousError + ' ➡️ Choix de difficulté invalide', "E005")


def invalidInscription(previousError=""):
    raise Exception(previousError + ' ➡️ Inscription invalide, impossible de continuer', "E004")


def insertionBaseDeDonneeError(previousError=""):
    raise Exception(previousError + ' ➡️ Erreur lors de l\'insertion dans la base de données', "E006")


def generationIdMaxError(previousError=""):
    raise Exception(previousError + ' ➡️ Erreur lors de la génération de l\'id max', "E007")


def nonConnecteHistoriqueError(previousError=""):
    raise Exception(previousError + ' ➡️ "Il faut être connecté pour accéder à l\'historique des parties.', "E008")
