-- Instructions permettant de créer la base de données
CREATE TABLE tentative (
    idPartie INTEGER,
    numLigne INTEGER,
    mot VARCHAR(50),
    CONSTRAINT idPartie_PK PRIMARY KEY (idPartie, numLigne),
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
    scorePartie INTEGER,
    CONSTRAINT idPartie_PK PRIMARY KEY (idPartie),
    CONSTRAINT parametre_FK FOREIGN KEY (parametre) REFERENCES parametre(id),
    CONSTRAINT motATrouver_FK FOREIGN KEY (motATrouver) REFERENCES dictionnaire(idMot)
);

CREATE TABLE parametre (
    id INTEGER,
    longueur INTEGER,
    nbEssais INTEGER,
    difficulte INTEGER,
    CONSTRAINT id_PK PRIMARY KEY (id)
);

CREATE TABLE utilisateur (
    idUtilisateur INTEGER,
    pseudo VARCHAR(50),
    password VARCHAR(50),
    parametreDernierePartie INTEGER,
    partieEnCours INTEGER,
    scoreUtilisateur INTEGER,
    CONSTRAINT idJoueur_PK PRIMARY KEY (idUtilisateur),
    CONSTRAINT parametreDernierePartie_FK FOREIGN KEY (parametreDernierePartie) REFERENCES partie(idPartie),
    CONSTRAINT partieEnCours_FK FOREIGN KEY (partieEnCours) REFERENCES partie(idPartie)
);

CREATE TABLE dictionnaire (
    idMot INTEGER,
    mot VARCHAR(50),
    longueur INTEGER,
    frequence INTEGER,
    CONSTRAINT idMot_PK PRIMARY KEY (idMot)
);
