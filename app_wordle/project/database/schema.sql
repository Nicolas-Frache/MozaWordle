-- Instructions permettant de créer la base de données

-- TODO


CREATE TABLE IF NOT EXISTS user (
pseudo text,
utilisateur_email text,
CONSTRAINT user_PK PRIMARY KEY (pseudo),
);