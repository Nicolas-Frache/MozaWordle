from tests import app


def test_test_get():
    """
    AVEC une application flask de test
    QUAND la page '/test' page est appellée (GET)
    ALORS on verifie que la page contient la chaine 'test'
    """
    with app.test_client() as test_client:
        response = test_client.get('/test')
        assert response.status_code == 200
        assert b"test" in response.data
