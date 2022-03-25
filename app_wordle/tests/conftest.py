import pytest as pytest


# Méthode qui s'execute automatiquement au début de l'éxecution de chaque méthode de test
# Voir: https://docs.pytest.org/en/6.2.x/fixture.html#autouse-fixtures-fixtures-you-don-t-have-to-request
#
# Réinitialise la base de données de test pour chaque classe de test
@pytest.fixture(autouse=True)
def reset_test_database():
    # TODO: Réinitialiser la base de données de test
    pass


# Supprime le fichier de la base de données de test à la fin de la session de test
# Voir: https://docs.pytest.org/en/6.2.x/fixture.html#adding-finalizers-directly
@pytest.fixture(scope="session", autouse=True)
def init_and_cleanup(request):
    def clean_db_test_file():
        pass
        # TODO: Supprimer le fichier de la base de données de test
    request.addfinalizer(clean_db_test_file)
