#include "projet.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QDebug>
#include <QRegularExpression>
#include <QPdfWriter>
#include <QPainter>
#include <QTableView>
#include <QAbstractItemModel>
#include <QFileDialog>
#include <QMessageBox>

Projet::Projet(const QString& nom,
               const QString& description,
               const QDate& dateDebut,
               const QDate& dateFin,
               const QString& statut)
    : m_nom(nom),
    m_description(description),
    m_dateDebut(dateDebut),
    m_dateFin(dateFin),
    m_statut(statut)
{
}


bool Projet::ajouter()
{
    QSqlQuery q;
    q.prepare(
        "INSERT INTO PROJET "
        "(ID_PROJET, NOM, DESCRIPTION, DATE_DEBUT, DATE_FIN, STATUT) "
        "VALUES (SEQ_PROJET.NEXTVAL, :nom, :desc, :d1, :d2, DEFAULT)"
        //                    ^^^^^^  on force l'utilisation du DEFAULT Oracle pour STATUT
        );

    q.bindValue(":nom",  m_nom);
    q.bindValue(":desc", m_description);
    q.bindValue(":d1",   m_dateDebut);
    q.bindValue(":d2",   m_dateFin);

    if (!q.exec()) {
        qDebug() << "[ajouter] SQL err:" << q.lastError().text();
        return false;
    }
    return true;
}




QSqlQueryModel* Projet::afficher()
{
    auto *model = new QSqlQueryModel();
    model->setQuery(
        "SELECT ID_PROJET AS ID, NOM, DESCRIPTION, DATE_DEBUT, DATE_FIN, STATUT "
        "FROM CONTENTCREATION.PROJET"
        );

    if (model->lastError().isValid())
        qDebug() << "[afficher] SQL err:" << model->lastError().text();

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Description"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date Début"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date Fin"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Statut"));
    return model;
}

bool Projet::modifier(int id,
                      const QString& nom,
                      const QString& description,
                      const QDate& dateDebut,
                      const QDate& dateFin,
                      const QString& statut)
{
    QSqlQuery q;
    q.prepare(
        "UPDATE PROJET "
        "SET NOM = :nom, DESCRIPTION = :desc, DATE_DEBUT = :dateDebut, DATE_FIN = :dateFin, STATUT = :statut "
        "WHERE ID_PROJET = :id"
        );

    q.bindValue(":nom", nom);
    q.bindValue(":desc", description);
    q.bindValue(":dateDebut", dateDebut);
    q.bindValue(":dateFin", dateFin);
    q.bindValue(":statut", statut);
    q.bindValue(":id", id);

    if (!q.exec()) {
        qDebug() << "[modifier] SQL err:" << q.lastError().text();
        return false;
    }

    return q.numRowsAffected() > 0;
}

bool Projet::supprimer(int id_)
{
    QSqlQuery q;
    q.prepare("DELETE FROM PROJET WHERE ID_PROJET = :id");
    q.bindValue(":id", id_);

    if (!q.exec()) {
        qDebug() << "[supprimer] SQL err:" << q.lastError().text();
        return false;
    }

    return q.numRowsAffected() > 0;
}
