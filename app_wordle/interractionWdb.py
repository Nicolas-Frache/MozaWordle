#import
import sqlite3
import string

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

def Generate_Max_Id(tables : string) -> int:
    try:
        connexion = sqlite3.connect(database)
        cursor = connexion.cursor()

        
        cursor.execute("SELECT max(id) FROM " +  tables) ## la la la les injections SQL ca existe pas
        new_id = cursor.fetchone()

        if new_id == None:
            new_id = [0]

        cursor.close()
        connexion.close()
        return new_id + 1
    except sqlite3.Error as error:
        print("Erreur lors de la génération d'un id max : ", error)



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