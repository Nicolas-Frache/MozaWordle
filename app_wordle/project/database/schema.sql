-- Instructions permettant de créer la base de données
CREATE TABLE tentative (
    idPartie INTEGER,
    numLigne INTEGER,
    mot VARCHAR(50),
    CONSTRAINT idPartie_PK PRIMARY KEY (idPartie),
    CONSTRAINT numLigne_PK PRIMARY KEY (numLigne),
    CONSTRAINT idPartie_FK FOREIGN KEY (idPartie) REFERENCES partie(idPartie),
    CONSTRAINT mot_FK FOREIGN KEY (mot) REFERENCES dictionnaire(idMot)
);

CREATE TABLE partie (
    idPartie INTEGER,
    parametre INTEGER,
    estEnCours INTEGER,
    idJoueur INTEGER,
    motATrouver VARCHAR(50),
    date DATE,
    tourActuel INTEGER,
    aGagne INTEGER,
    CONSTRAINT idPartir_PK PRIMARY KEY (idPartie),
    CONSTRAINT parametre_FK FOREIGN KEY (parametre) REFERENCES parametre(id),
    CONSTRAINT motATrouver_FK FOREIGN KEY (motATrouver) REFERENCES dictionnaire(idMot)
);

CREATE TABLE parametre (
    id INTEGER,
    longueur INTEGER,
    nbEssais INTEGER,
    CONSTRAINT id_PK PRIMARY KEY (id)
);

CREATE TABLE utilisateur (
    idJoueur INTEGER,
    parametreDernierePartie INTEGER,
    partieEnCours INTEGER,
    CONSTRAINT idJoueur_PK PRIMARY KEY (idJoueur),
    CONSTRAINT parametreDernierePartie_FK FOREIGN KEY (parametreDernierePartie) REFERENCES partie(idPartie),
    CONSTRAINT partieEnCours_FK FOREIGN KEY (partieEnCours) REFERENCES partie(idPartie)
);

CREATE TABLE dictionnaire (
    idMot INTEGER,
    type VARCHAR(50),
    mot VARCHAR(50),
    longueur INTEGER,
    langue VARCHAR(50),
    CONSTRAINT idMot_PK PRIMARY KEY (idMot)
);