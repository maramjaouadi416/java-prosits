#include "connection.h"
#include <QSqlDatabase>

// Initialization of the instance pointer to nullptr
Connection* Connection::p_instance = nullptr;

// Private constructor
Connection::Connection()
{
    // Connection initialization (if necessary)
}

// Static method to obtain the unique instance of the Connection class
Connection* Connection::instance()
{
    if (p_instance == nullptr) {
        p_instance = new Connection();
    }
    return p_instance;
}

// Method to establish the database connection
bool Connection::createConnect()
{
    // ➕ ÉTAPE 1 : Supprimer la connexion par défaut (nom = chaîne vide)
    if (QSqlDatabase::contains()) {
        QSqlDatabase::removeDatabase("");
    }

    // ➕ ÉTAPE 2 : Créer une nouvelle connexion propre


    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("smartcontentcreationstore");
    db.setUserName("contentcreation");
    db.setPassword("esprit18");


    if (db.open()) {
        qDebug() << "✅ Connexion Oracle établie avec succès.";
        return true;
    } else {
        qDebug() << "❌ Erreur de connexion :" << db.lastError().text();
        return false;
    }
}

// Private destructor to close the database connection
Connection::~Connection()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (db.isOpen()) {
        db.close();
    }
}
