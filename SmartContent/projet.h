#ifndef GPROJET_H
#define GPROJET_H

#include <QString>
#include <QDate>
#include <QSqlQueryModel>
#include <QTableView>

class Projet
{
public:
    Projet() = default;
    Projet(const QString& nom,
           const QString& description,
           const QDate&   dateDebut,
           const QDate&   dateFin,
           const QString& statut);

    bool ajouter();
    bool supprimer(int id_);
    bool modifier(int id,
                  const QString& nom,
                  const QString& description,
                  const QDate&   dateDebut,
                  const QDate&   dateFin,
                  const QString& statut);
    QSqlQueryModel* afficher();


private:
    int     m_id = -1;
    QString m_nom;
    QString m_description;
    QDate   m_dateDebut;
    QDate   m_dateFin;
    QString m_statut;
};

#endif // GPROJET_H
