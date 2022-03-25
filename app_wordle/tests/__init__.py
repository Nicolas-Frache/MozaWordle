# Creation de l'app au lancement des test

def create_app():
    from app import app
    return app


app = create_app()
app.app_context().push()
