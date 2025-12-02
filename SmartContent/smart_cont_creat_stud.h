#ifndef SMART_CONT_CREAT_STUD_H
#define SMART_CONT_CREAT_STUD_H

#include <QMainWindow>
#include <QTableWidget>
#include <QSqlQueryModel>
#include <QPointer>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "ui_smart_cont_creat_stud.h"
#include "employes.h"
#include "evenement.h"
#include "projet.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SmartContentCreationStore; }
QT_END_NAMESPACE

class SmartContentCreationStore : public QMainWindow
{
    Q_OBJECT

public:
    SmartContentCreationStore(QWidget *parent = nullptr);
    ~SmartContentCreationStore();
    bool exporter(QTableView *tableView, const QString &title);
private slots:
    // === Chatbot (SCS.AI) ===
    void sendToGemma(const QString& userQuestion, const QString& fullPrompt, double temperature = 0.7);
    void handleProjectRelatedQuestion(const QString& question);
    void handleProjectRecommendation(const QString& question);
    void handleProjectListRequest(const QString& question);

    // --------------------- Connexion ---------------------
    void on_btn_connect_clicked();
    void on_btn_retour_menu_login_clicked();

    // --------------------------------------------------------- Employés --------------------------------------------------------------

    void on_btn_retour_menu_login();
    void on_btnretour_clicked();
    /*void on_btn_faceid_clicked();*/

    void on_pushButton_ajouter_clicked();
    void on_pushButton_chercher_clicked();
    void on_pushButton_modif_clicked();
    void on_pushButton_supprimer_clicked();
    void on_pushButton_retour_menu_clicked();
    void on_btn_clear_clicked();
    void on_btn_tri_date_clicked();

    void showEmployeeRoleChart();

    void updateEmployeCountLabel();

    void on_btn_motdepasse_oublie_clicked();
    /*
    QVideoWidget *videoWidget = nullptr;
    QCamera *camera = nullptr;
    QMediaCaptureSession *captureSession = nullptr;

    // --- Face ID addition ---
    QImageCapture *imageCapture = nullptr;
    QTimer *faceIdTimer = nullptr;
    QLabel *faceDisplayLabel = nullptr; // For showing processed camera frames with rectangles

    // Member variables for FaceID
    QWidget* cameraWindow = nullptr;
    */
    // Face ID



    // -------------------------------------------------------- Événements --------------------------------------------------------
    void on_btn_ajout_clicked();
    void on_btn_modifier_clicked();
    void on_btn_supprimer_clicked();
    void on_tableWidgetevent_itemSelectionChanged();
    void on_pushButtoneventtrier_9_clicked();
    void on_pushButtoneventrech_12_clicked();
    void on_pushButtoneventpdf_2_clicked();

    void on_tridateproj_2_clicked();
    void on_export_2gprojets_2_clicked();
private:
    Ui::SmartContentCreationStore *ui;
    employes Etmp;
    Evenement Eevent;
    int m_currentSelectedEventID = -1;
    int m_editingEventId = -1;

    void refreshEventTable();
    void fillEventFormFromRow(int row);
    void updateEventStats();
    void verifierRappelsEvenements(); // ← Notification SANS IA

    // ➕ Cache dynamique des valeurs uniques dans EVENEMENT
    QList<int> m_idsEvenements;
    QStringList m_nomsEvenements;
    QStringList m_typesEvenements;
    QStringList m_lieuxEvenements;

    // ➕ Connaissances depuis fichiers texte
    QMap<QString, QStringList> m_connaissances;
    QMap<QString, QString> m_memoire;

    // ➕ Fonctions du chatbot
    void chargerValeursUniquesEvenements();
    void chargerConnaissances();
    void chargerMemoire();
    QString repondreDepuisBase(const QString& question);
    QString repondreDepuisFichiers(const QString& question);
    void apprendreNouvelleReponse(const QString& question, const QString& reponse);
    QString repondreQuestionEvenement(const QString& question);



    void initGestionProjet();
    void refreshProjetTable(const QString& nomLike = QString());
    void onAjouterProjet();
    void onModifierProjet();
    void onSupprimerProjet();
    void onSearchProjet(const QString& text);
    void loadProjetToForm();
    void afficherStatistiques();
    QPointer<QLineEdit>   gp_leSearch_;
    QPointer<QLineEdit>   gp_leNom_;
    QPointer<QLineEdit>   gp_leDesc_;
    QPointer<QComboBox>   gp_cbStatut;
    QPointer<QDateEdit>   gp_deDebut_;
    QPointer<QDateEdit>   gp_deFin_;
    QPointer<QTableView>  gp_table_;
    QPointer<QPushButton> gp_btnAdd_;
    QPointer<QPushButton> gp_btnEdit_;
    QPointer<QPushButton> gp_btnDel_;

    QSqlQueryModel* gp_model_ = nullptr;

    // === Chatbot Networking ===
    QNetworkAccessManager *m_networkManager;
    QTimer *m_dotsTimer;
    int m_dotCount;
};

QString hashPassword(const QString& password);

#endif // SMART_CONT_CREAT_STUD_H
