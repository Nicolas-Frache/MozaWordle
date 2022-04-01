def connexionBaseDeDonneeError():
    raise Exception("Erreur lors de la connexion à la base de données", "E001")


def requetageBaseDeDonneeError():
    raise Exception("Erreur lors du requêtage de la base de données", "E002")
