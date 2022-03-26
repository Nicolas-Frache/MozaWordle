#import
import sqlite3

#DB name
database = "project/database/database.db"

def Basic_Query(sql, param_sql, error_msg):
    try:
        connexion = sqlite3.connect(database)
        cursor = connexion.cursor()

        cursor.execute(sql,param_sql)
        query = cursor.fetchall()

        cursor.close()
        connexion.close()
        return query
    except sqlite3.Error as error:
        print(error_msg, error)


def Create_DB():
    try:
        connexion = sqlite3.connect(database)
        cursor = connexion.cursor()

        # Création  des tables
        with open("project/database/schema.sql") as file:
            sql = file.read()
            cursor.executescript(sql)
        
        #Insertion des données
        
        cursor.close()
        connexion.close()
        return None
    except sqlite3.Error as error:
        print("Erreur lors de la création de DB : ", error)