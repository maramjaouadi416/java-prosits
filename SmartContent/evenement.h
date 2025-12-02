#ifndef EVENEMENT_H
#define EVENEMENT_H

#include <QString>
#include <QTableWidget>
#include <QDate>
#include <QPair>

class QChartView;

class Evenement
{
    int id;
    QString nom;
    QString type;
    QString debut; // format: "yyyy-MM-dd"
    QString fin;   // format: "yyyy-MM-dd"
    QString lieu;

public:
    // Constructeurs
    Evenement();
    Evenement(const QString &nom, const QString &type,
              const QString &debut, const QString &fin,
              const QString &lieu);

    // Getters
    int getID() const { return id; }
    QString getNom() const { return nom; }
    QString getType() const { return type; }
    QString getDebut() const { return debut; }
    QString getFin() const { return fin; }
    QString getLieu() const { return lieu; }

    // Setters
    void setID(int i) { id = i; }
    void setNom(const QString &n) { nom = n; }
    void setType(const QString &t) { type = t; }
    void setDebut(const QString &d) { debut = d; }
    void setFin(const QString &f) { fin = f; }
    void setLieu(const QString &l) { lieu = l; }

    // Méthodes base de données (CRUD + affichage)
    bool ajouter();
    static bool supprimer(int id);
    static bool modifier(int id, const QString &nom, const QString &type,
                         const QString &debut, const QString &fin,
                         const QString &lieu);
    static bool chargerDonnees(QTableWidget *table);
    static bool chargerDonneesTrieParType(QTableWidget *table, const QString &typeCible);
    static bool rechercherParNomSQL(QTableWidget *table, const QString &nomRecherche);

    // 🔹 Chargement des valeurs uniques pour le cache du chatbot
    static QList<int> recupererTousIDs();
    static QStringList recupererTousNomsUniques();
    static QStringList recupererTousTypesUniques();
    static QStringList recupererTousLieuxUniques();

    // 🔹 Réponses basées sur la base de données (pour repondreDepuisBase)
    static QString repondreParID(int id);
    static QString repondreParNom(const QString &nom);
    static QString repondreParType(const QString &type);
    static QString repondreParLieu(const QString &lieu);
    static QString repondreParDate(const QDate &date);
    static int compterTousEvenements();
    static QString listerTousEvenements();
    static QString listerEvenementsAFutur(const QDate &aujourdhui);
    static QString listerEvenementsAujourdhui(const QDate &aujourdhui);

    // 🔹 Notifications (rappels)
    static QList<QPair<QString, QDate>> recupererEvenementsImminents(int nbJours = 3);

    // Statistiques
    static void StatistiquesParType(QChartView *chartView);

    // Exporter
    static bool exporterTousEvenementsEnPDF(const QString &filePath);
};

#endif // EVENEMENT_H
