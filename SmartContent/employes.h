    #ifndef EMPLOYES_H
#define EMPLOYES_H

#include <QMainWindow>
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>

QT_BEGIN_NAMESPACE
namespace Ui {
class employes;
}
QT_END_NAMESPACE

class employes : public QMainWindow
{
    Q_OBJECT

private:
    Ui::employes *ui;

    int id;
    QString nom;
    QString prenom;
    QString email;
    QString mot_passe;
    float salaire;
    QDate date_rec;
    QString role;
    QString question_rep;

public:
    employes();
    employes(int, QString, QString, QString, QString, float, QDate, QString, QString);

    // Getters
    int getId() const { return id; }
    QString getNom() const { return nom; }
    QString getPrenom() const { return prenom; }
    QString getEmail() const { return email; }
    QString getMot_passe() const { return mot_passe; }
    float getSalaire() const { return salaire; }         // ← FIXED: was int
    QDate getDate_rec() const { return date_rec; }
    QString getRole() const { return role; }
    QString getQuestion_rep() const { return question_rep; }

    // Setters
    void setId(int i) { id = i; }
    void setNom(QString n) { nom = n; }
    void setPrenom(QString p) { prenom = p; }
    void setEmail(QString em) { email = em; }
    void setMot_passe(QString mp) { mot_passe = mp; }
    void setSalaire(float s) { salaire = s; }
    void setDate_rec(QDate dr) { date_rec = dr; }
    void setrole(QString r) { role = r; }
    void setquestion_rep(QString qr) { question_rep = qr; }
    bool btn_connect(const QString& login, const QString& passwordHash);
    int getEmployeeCount();

    QSqlQueryModel* afficher_stats();
    // CRUD operations
    bool ajouter();
    bool chercher(int);
    bool modifier(int);
    QSqlQueryModel* afficher();
    QSqlQueryModel* afficher_tri_date();
    bool supprimer(int);
    QSqlQueryModel* affichercherch(int id);

    bool verifierQuestionSecurite(const QString& login, const QString& reponse);
    QString genererNouveauMotDePasse();
    bool mettreAJourMotDePasse(const QString& login, const QString& nouveauHash);

    // Retourne true si trouvé, et remplit questionTexte + reponseStockee
    bool getQuestionEtReponse(const QString& login,
                              QString& questionTexte,
                              QString& reponseStockee);

};

#endif // EMPLOYES_H
