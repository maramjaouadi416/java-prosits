#include "employes.h"
#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>

employes::employes()
{
    // Default initialization if needed
}

employes::employes(int id, QString nom, QString prenom, QString email, QString mot_passe,
                   float salaire, QDate date_rec, QString role, QString question_rep)
{
    this->id = id;
    this->nom = nom;
    this->prenom = prenom;
    this->email = email;
    this->mot_passe = mot_passe;
    this->salaire = salaire;
    this->date_rec = date_rec;
    this->role = role;
    this->question_rep = question_rep;
}


bool employes::ajouter(){
    QSqlQuery query;
    qDebug() << "Inserting values:" << id << nom << prenom << email;

    query.prepare("INSERT INTO EMPLOYEE (EMPLOYEE_ID, NOM, PRENOM, EMAIL, MOT_DE_PASSE, SALAIRE, RECRUTEMENT, ROLE, QUESTION_REP) "
                  "VALUES (:id, :nom, :prenom, :email, :mot_passe, :salaire, TO_DATE(:date_rec, 'DD/MM/YYYY'), :role, :question_rep)");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":mot_passe", mot_passe);
    query.bindValue(":salaire", salaire);

    // Make sure date_rec is a QDate or QDateTime, and format as string for Oracle TO_DATE
    query.bindValue(":date_rec", date_rec.toString("dd/MM/yyyy")); // Or use "dd/MM/yyyy HH:mm" for datetime

    query.bindValue(":role", role);
    query.bindValue(":question_rep", question_rep);


    if (!query.exec()) {
        qDebug() << "Insert failed:" << query.lastError().text();
        return false;
    }
    qDebug() << "Insert successful!";
    return true;
}

bool employes::chercher(int id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYEE WHERE EMPLOYEE_ID = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Recherche échouée:" << query.lastError().text();
        return false;
    }

    return query.next(); // true if found
}
QSqlQueryModel* employes::affichercherch(int id)
{
    QSqlQueryModel* model = new QSqlQueryModel();

    // Build the SQL string with the id directly (safe for int)
    QString sql = QString("SELECT EMPLOYEE_ID, NOM, PRENOM, MOT_DE_PASSE, EMAIL, SALAIRE, RECRUTEMENT, ROLE, QUESTION_REP FROM EMPLOYEE WHERE EMPLOYEE_ID = %1").arg(id);

    model->setQuery(sql);

    if (model->lastError().isValid()) {
        qDebug() << "Query error:" << model->lastError().text();
        // (Optional: show message to user or take corrective action)
    }

    return model;
}


QSqlQueryModel* employes::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT EMPLOYEE_ID, NOM, PRENOM, MOT_DE_PASSE, EMAIL, SALAIRE, RECRUTEMENT, ROLE, QUESTION_REP FROM EMPLOYEE");
    if (!model)
        model = new QSqlQueryModel();

    model->setQuery("SELECT EMPLOYEE_ID, NOM, PRENOM, MOT_DE_PASSE, EMAIL, SALAIRE, RECRUTEMENT, ROLE, QUESTION_REP FROM EMPLOYEE");

    if (model->lastError().isValid()) {
        qDebug() << "Query error:" << model->lastError().text();
    }
    return model;
}



QSqlQueryModel* employes::afficher_tri_date()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT EMPLOYEE_ID, NOM, PRENOM, MOT_DE_PASSE, EMAIL, SALAIRE, RECRUTEMENT, ROLE, QUESTION_REP FROM EMPLOYEE ORDER BY RECRUTEMENT ASC");

    if (model->lastError().isValid()) {
        qDebug() << "Query error:" << model->lastError().text();
        return model;
    }
    return model;
}





bool employes::modifier(int id)
{
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM EMPLOYEE WHERE EMPLOYEE_ID = :id");
    checkQuery.bindValue(":id", id);
    if (!checkQuery.exec() || !checkQuery.next() || checkQuery.value(0).toInt() == 0) {
        qDebug() << "ID not found. No update performed.";
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE EMPLOYEE SET NOM = :nom, PRENOM = :prenom, EMAIL = :email, "
                  "MOT_DE_PASSE = :mot_passe, SALAIRE = :salaire, RECRUTEMENT = TO_DATE(:date_rec, 'DD/MM/YYYY'), "
                  "ROLE = :role, QUESTION_REP = :question_rep WHERE EMPLOYEE_ID = :id");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":mot_passe", mot_passe);
    query.bindValue(":salaire", salaire);
    query.bindValue(":date_rec", date_rec.toString("dd/MM/yyyy"));
    query.bindValue(":role", role);
    query.bindValue(":question_rep", question_rep);

    if (!query.exec()) {
        qDebug() << "Update failed:" << query.lastError().text();
        return false;
    }

    qDebug() << "Update successful!";
    return true;
}

bool employes::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM EMPLOYEE WHERE EMPLOYEE_ID = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Delete failed:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool employes::btn_connect(const QString& login, const QString& passwordHash) {
    qDebug() << "btn_connect login =" << login;
    qDebug() << "btn_connect hash  =" << passwordHash;

    // login is "prenom nom"
    QStringList parts = login.split(' ', Qt::SkipEmptyParts);
    if (parts.size() < 2) {
        qDebug() << "Invalid login format, expected 'prenom nom':" << login;
        return false;
    }

    QString prenom = parts[0];  // "youssef"
    QString nom    = parts[1];  // "ismail"
    QSqlQuery test;
    test.exec("SELECT COUNT(*) FROM EMPLOYEE");
    if (test.next()) {
        qDebug() << "EMPLOYEE rows seen from Qt =" << test.value(0).toInt();
    }

    QSqlQuery query;
    query.prepare(R"(
        SELECT EMPLOYEE_ID
        FROM EMPLOYEE
        WHERE PRENOM = :prenom
          AND NOM    = :nom
          AND MOT_DE_PASSE = :pwd
    )");
    query.bindValue(":prenom", prenom);
    query.bindValue(":nom",    nom);
    query.bindValue(":pwd",    passwordHash);

    if (!query.exec()) {
        qDebug() << "Login query failed:" << query.lastError().text();
        return false;
    }

    if (!query.next()) {
        qDebug() << "No match for prenom =" << prenom
                 << "nom =" << nom
                 << "hash =" << passwordHash;
        return false;
    }

    return true;
}



int employes::getEmployeeCount() {
    QSqlDatabase db = QSqlDatabase::database();  // default DB connection
    if (!db.isOpen() && !db.open()) {
        qDebug() << "DB open failed:" << db.lastError().text();
        return -1; // error indicator
    }

    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM EMPLOYEE");

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    } else {
        qDebug() << "Query error:" << query.lastError().text();
        return -1;
    }
}

bool employes::verifierQuestionSecurite(const QString& login, const QString& reponse) {
    QStringList parties = login.split(" ", Qt::SkipEmptyParts);
    if (parties.size() != 2) return false;

    QString prenom = parties[0], nom = parties[1];

    QSqlQuery query;
    query.prepare("SELECT QUESTION_REP FROM EMPLOYEE WHERE PRENOM = :prenom AND NOM = :nom");
    query.bindValue(":prenom", prenom);
    query.bindValue(":nom", nom);

    if (!query.exec() || !query.next()) {
        qDebug() << "Utilisateur non trouvé pour réinitialisation";
        return false;
    }

    QString reponseStockee = query.value(0).toString().trimmed();
    return reponse.toLower() == reponseStockee.toLower();
}


bool employes::mettreAJourMotDePasse(const QString& login, const QString& nouveauHash) {
    QStringList parties = login.split(" ", Qt::SkipEmptyParts);
    if (parties.size() != 2) return false;

    QString prenom = parties[0], nom = parties[1];

    QSqlQuery query;
    query.prepare("UPDATE EMPLOYEE SET MOT_DE_PASSE = :hash WHERE PRENOM = :prenom AND NOM = :nom");
    query.bindValue(":hash", nouveauHash);
    query.bindValue(":prenom", prenom);
    query.bindValue(":nom", nom);

    return query.exec();
}


bool employes::getQuestionEtReponse(const QString& login,
                                    QString& questionTexte,
                                    QString& reponseStockee)
{
    QStringList parties = login.split(" ", Qt::SkipEmptyParts);
    if (parties.size() != 2)
        return false;

    QString prenom = parties[0];
    QString nom    = parties[1];

    QSqlQuery query;
    query.prepare("SELECT QUESTION_REP FROM EMPLOYEE "
                  "WHERE PRENOM = :prenom AND NOM = :nom");
    query.bindValue(":prenom", prenom);
    query.bindValue(":nom", nom);

    if (!query.exec() || !query.next()) {
        qDebug() << "Login inconnu pour QUESTION_REP";
        return false;
    }

    QString qr = query.value(0).toString().trimmed(); // "question reponse"

    int lastSpace = qr.lastIndexOf(' ');
    if (lastSpace == -1) {
        qDebug() << "QUESTION_REP mal formé:" << qr;
        return false;
    }

    questionTexte  = qr.left(lastSpace).trimmed();
    reponseStockee = qr.mid(lastSpace + 1).trimmed();
    return true;
}
