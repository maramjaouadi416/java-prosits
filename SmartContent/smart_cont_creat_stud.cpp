#include "smart_cont_creat_stud.h"
#include "ui_smart_cont_creat_stud.h"
#include "chatbot.h"
#include "connection.h" // 👈 Ajouté pour accéder à la connexion
#include "employes.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QCryptographicHash>
#include <QDate>
#include <QChartView>
#include <QPixmap>
#include <QDebug>
#include <QFileDialog>
#include <QScreen>
#include <QRandomGenerator>
#include <QFile>
#include <QTextStream>
#include <QTime>

#include <QToolBar>
#include <QDockWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QPixmap>
#include <QAction>
#include <QPainter>
#include <QBitmap>
#include <QRegularExpression>
#include <QMessageBox>
#include <QHeaderView>
#include <QPdfWriter>
#include <QFileDialog>
#include <QSqlError>
#include <QCryptographicHash>
#include <QInputDialog>
#include <QPieSeries>
#include <QScrollBar>


#include <QScreen>
#include <QGuiApplication>

#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QChart>
#include <QDialog>
#include <QVBoxLayout>
#include <QtCharts/QValueAxis>

#include <QMessageBox>
#include <QTimer>
#include <QDebug>



// Hash helper function //hachage CHA-256
QString hashPassword(const QString& password) {
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return QString(hash.toHex());
}

// ---------------------- Constructor ----------------------
SmartContentCreationStore::SmartContentCreationStore(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::SmartContentCreationStore),
    m_networkManager(new QNetworkAccessManager(this)),
    m_dotsTimer(new QTimer(this)),
    m_dotCount(0)
{

    qDebug() << "Graphics Backend:" << QGuiApplication::platformName();
    ui->setupUi(this);
    // ====== CHATBOT SETUP (HTTP DIRECT) ======
    connect(m_dotsTimer, &QTimer::timeout, this, [this]() {
        QString dots = QString(".").repeated(m_dotCount % 4);
        ui->ChatResponse->setText("⏳ Réflexion" + dots);
        m_dotCount++;
    });

    // Connect chatButton to rich lambda (user + AI HTML messages)
    connect(ui->chatButton, &QPushButton::clicked, this, [this]() {
        QString question = ui->userInput->text().trimmed();
        if (question.isEmpty()) return;

        // Show user message immediately
        QString userHtml = QString(
                               "<div style='text-align:right; margin:8px 0;'>"
                               "<span style='background-color:#E0F7FA; color:black; padding:10px 14px; "
                               "border-radius:14px; display:inline-block; max-width:80%; word-wrap:break-word;'>"
                               "<b>👤 You</b><br>%1"
                               "</span>"
                               "</div>"
                               ).arg(question.toHtmlEscaped());
        ui->ChatResponse->setHtml(userHtml);

        // Start thinking animation
        m_dotCount = 0;
        m_dotsTimer->start(400);

        // 🔍 Enhanced regex for recommendations (detailed project advice)
        QRegularExpression reRecommend(R"((recommandation|conseil|suggestion|avis|plan d'action|que dois-je faire|comment améliorer|gestion de|projet\s*d'?id|nom\s*de\s*projet|date\s*début|date\s*fin|meilleur|analyse|rapport|bilan|performance|statistiques|pourcentage|améliorer|optimiser|solution|proposition|stratégie))",
                                       QRegularExpression::CaseInsensitiveOption);

        // 🔍 Enhanced regex for general project data (IDs, names, stats, lists)
        QRegularExpression reProject(R"((projet|projects?|id|ids|identifiant|numéro|nom|noms|name|names|statut|status|état|date\s*début|début|start\s*date|date\s*fin|fin|end\s*date|retard|en retard|delay|terminé|complété|completed|en cours|in progress|combien|how many|liste|list|affiche|show|donne|give|taux|rate|percentage|pourcentage))",
                                     QRegularExpression::CaseInsensitiveOption);

        // 🧠 Routing logic
        if (question.contains(reRecommend)) {
            handleProjectRecommendation(question);
            return;
        }
        if (question.contains(reProject)) {
            handleProjectRelatedQuestion(question);
            return;
        }

        // 💬 Free general chat mode (no restrictions, multilingual, creative)
        QString generalPrompt =
            "You are SCS.AI, a smart and friendly AI assistant.\n"
            "You can speak any language the user uses (French, English, Arabic, etc.).\n"
            "You're helpful, creative, and natural — like a real human.\n"
            "You can answer any question: jokes, facts, advice, explanations, stories, or casual chat.\n"
            "Only when asked about projects, employees, or events do you use structured data.\n"
            "Otherwise, be engaging, warm, and unrestricted.\n\n"
            "User: " + question + "\n\n"
                         "SCS.AI:";

        sendToGemma(question, generalPrompt, 1.1); // High creativity
    });

    // Enter key support
    connect(ui->userInput, &QLineEdit::returnPressed, ui->chatButton, &QPushButton::click);
    // 🔒 Nettoyer toute connexion ODBC résiduelle AVANT setupUi
    if (QSqlDatabase::contains("")) {
        QSqlDatabase db = QSqlDatabase::database("");
        if (db.isOpen()) db.close();
        QSqlDatabase::removeDatabase("");
    }
    // Example: Set color and font in code for visibility
    ui->nbEmploys_label->setStyleSheet("color: #000; background: #fff; font-size: 16px;");
    ui->nbEmploys_label->setAlignment(Qt::AlignCenter);


    // Get primary screen geometry and set window size proportionally
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    int width = screenGeometry.width() * 0.8;   // 80% of available width
    int height = screenGeometry.height() * 0.8; // 80% of available height

    this->resize(width, height);

    // Optional: Set minimum size for usability
    this->setMinimumSize(600, 400);

    // ✅ Ajustement à la taille de l'écran
    {
        QRect screenGeometry = QGuiApplication::primaryScreen()->availableGeometry();
        int maxWidth = qMin(1400, static_cast<int>(screenGeometry.width() * 0.95));
        int maxHeight = qMin(900, static_cast<int>(screenGeometry.height() * 0.95));
        this->resize(maxWidth, maxHeight);
        this->move(
            (screenGeometry.width() - maxWidth) / 2,
            (screenGeometry.height() - maxHeight) / 2
            );

        if (ui->tableWidgetevent) {
            ui->tableWidgetevent->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        }
        if (ui->chatviewevent) {
            ui->chatviewevent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            ui->chatviewevent->setMinimumSize(300, 200);
        }
    }

    ui->nbEmploys_label->setAlignment(Qt::AlignCenter);

    //----------------------------------control saisi----------------------------------------------------
    //----------------------------------control saisi----------------------------------------------------
    ui->lineEdit_Id->setValidator(new QIntValidator(0, 99999999, this));
    QDoubleValidator *validator = new QDoubleValidator(0, 1000000, 2, this);
    validator->setLocale(QLocale::c());  // Use "C" locale (decimal point)
    ui->lineEdit_salaire->setValidator(validator);

    // Pour les champs texte (noms, prénoms, rôle, question/réponse)
    // Accepter uniquement les lettres, espaces et '-'
    QRegularExpression reAlpha("^[A-Za-zÀ-ÿ\\s'-]+$");
    ui->lineEdit_nom->setValidator(new QRegularExpressionValidator(reAlpha, this));
    ui->lineEdit_prenom->setValidator(new QRegularExpressionValidator(reAlpha, this));
    ui->lineEdit_role->setValidator(new QRegularExpressionValidator(reAlpha, this));
    ui->lineEdit_questionrep->setValidator(new QRegularExpressionValidator(reAlpha, this));

    // Pour l'email : validation basique permet lettres, chiffres, . _ - et @
    QRegularExpression reMail("^[A-Za-z0-9._-]+@[A-Za-z0-9._-]+\\.[A-Za-z]{2,}$");
    ui->lineEdit_email->setValidator(new QRegularExpressionValidator(reMail, this));

    // Pour le mot de passe, au moins 4 caractères, aucune restriction sur le type ici mais tu peux adapter !
    ui->lineEdit_motpasse->setMaxLength(30); // Limite longueur éventuelle

    initGestionProjet();
    refreshEventTable();


    // Load model
    QSqlQueryModel* model = Etmp.afficher();
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->hide();
    ui->tableView->verticalHeader()->hide();

    qDebug() << "[Debug] model rows:" << model->rowCount();
    qDebug() << "[Debug] model cols:" << model->columnCount();
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QVariant v = model->data(model->index(row, col));
            qDebug() << "[cell]" << row << col << v << v.toString();
        }
    }
    //------------------------------Menu----------------------------------------
    // gestion employes
    connect(ui->btn_Gemp, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget1->setCurrentIndex(1);


    });
    // gestion project
    connect(ui->btn_Gproj, &QPushButton::clicked, this, [=]() {



    });
    // gestion ressources
    connect(ui->btn_Gres, &QPushButton::clicked, this, [=]() {


    });
    // gestion evenements
    connect(ui->btn_Geven, &QPushButton::clicked, this, [=]() {



    });
    // gestion contenu
    connect(ui->btn_Gcont, &QPushButton::clicked, this, [=]() {


    });
    // gestion client
    connect(ui->btn_Gclient, &QPushButton::clicked, this, [=]() {



    });




    // Go to second page emp of ajouter/modifier
    connect(ui->btn_ajoutmodif_emp, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget_2->setCurrentIndex(1);
        ui->btn_ajoutmodif_emp->setFocus();

    });
    showEmployeeRoleChart();



    // Hide headers
    ui->tableView->horizontalHeader()->hide();
    ui->tableView->verticalHeader()->hide();  // optional

    // DEBUG - Show model dimensions
    qDebug() << "[Debug] model rows:" << model->rowCount();
    qDebug() << "[Debug] model cols:" << model->columnCount();

    // DEBUG - Print every cell
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QVariant v = model->data(model->index(row, col));
            qDebug() << "[cell]" << row << col << v << v.toString();
        }
    }

    //-------------export_button-----------------
    connect(ui->btn_export_emp, &QPushButton::clicked, this, [this]() {
        QString fileName = QFileDialog::getSaveFileName(this, "Exporter en TXT", "", "Text Files (*.txt)");
        if (fileName.isEmpty())
            return;

        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier pour écriture.");
            return;
        }

        QTextStream out(&file);

        // Remplace par ton modèle (QSqlQueryModel* model ou QAbstractItemModel* model)
        QAbstractItemModel* model =ui->tableView->model();

        int rows = model->rowCount();
        int cols = model->columnCount();

        // Export des entêtes
        QStringList headerCells;
        for (int c = 0; c < cols; ++c)
            headerCells << model->headerData(c, Qt::Horizontal).toString();
        out << headerCells.join("\t") << "\n";

        // Export de chaque ligne
        for (int r = 0; r < rows; ++r) {
            QStringList rowCells;
            for (int c = 0; c < cols; ++c) {
                rowCells << model->index(r, c).data().toString();
            }
            out << rowCells.join("\t") << "\n";
        }

        file.close();
        QMessageBox::information(this, "Succès", "Export terminé !");
    });
    //----------------------------------------------------images-------------------------------------------------------------
    QPixmap pixmap(":/images/images/sort.png");
    ui->sort_img->setScaledContents(true);
    ui->sort_img->setPixmap(pixmap);

    QPixmap pixmap2(":/images/images/export.png");
    ui->export_img->setScaledContents(true);
    ui->export_img->setPixmap(pixmap2);

    QPixmap pixmap4(":/images/images/user logo.png");
    ui->ident_img->setScaledContents(true);
    ui->ident_img->setPixmap(pixmap4);

    QPixmap pixmap5(":/images/images/employelogo.png");
    ui->nbEmploys_img->setScaledContents(true);
    ui->nbEmploys_img->setPixmap(pixmap5);

    QPixmap pixmap6(":/images/images/pngtree-vector-performance-icon-png-image_908499-removebg-preview.png");
    ui->roles_img->setScaledContents(true);
    ui->roles_img->setPixmap(pixmap6);

    QPixmap pixmap7(":/images/success.png");
    ui->performence_img->setScaledContents(true);
    ui->performence_img->setPixmap(pixmap7);

    QPixmap pixmap8(":/images/user logo.png");
    ui->profil_img->setScaledContents(true);
    ui->profil_img->setPixmap(pixmap8);

    // ---------------------- Événements setup ----------------------
    ui->tableWidgetevent->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    ui->tableWidgetevent->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetevent->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->btn_modifier->setEnabled(false);
    refreshEventTable();

    connect(ui->tableWidgetevent, &QTableWidget::itemSelectionChanged,
            this, &SmartContentCreationStore::on_tableWidgetevent_itemSelectionChanged);

    // ---------------------- Contrôle des dates ----------------------
    ui->dateEditeventdebut->setMinimumDate(QDate::currentDate());
    ui->dateEditeventfin->setMinimumDate(QDate::currentDate());
    connect(ui->dateEditeventdebut, &QDateEdit::dateChanged,
            ui->dateEditeventfin, &QDateEdit::setMinimumDate);

    // ---------------------- Sidebar Navigation ----------------------
    connect(ui->Gemp, &QPushButton::clicked, this, [=]() {
        ui->stackedWidgetemp->setCurrentIndex(0);
    });
    connect(ui->Gevent, &QPushButton::clicked, this, [=]() {
        ui->stackedWidgetemp->setCurrentIndex(2);
    });
    connect(ui->Gproj, &QPushButton::clicked, this, [=]() {
        ui->stackedWidgetemp->setCurrentIndex(1); // Page Projet
    });

    connect(ui->btn_retour_menu_login, &QPushButton::clicked, this, &SmartContentCreationStore::on_btn_retour_menu_login_clicked);

    // ---------------------- Buttons Employés ----------------------
    //-----------------------------------------------------button_connect-------------------------------------------------------------
    /*connect(ui->btn_connect, &QPushButton::clicked, this, &SmartContentCreationStore::on_btn_connect_clicked);*/
    connect(ui->btn_retour_menu_login, &QPushButton::clicked, this, &SmartContentCreationStore::on_btn_retour_menu_login);
    connect(ui->btnretour, &QPushButton::clicked, this, &SmartContentCreationStore::on_btnretour_clicked);
    connect(ui->btn_retour_menu_emp, &QPushButton::clicked, this, &SmartContentCreationStore::on_pushButton_retour_menu_clicked);




    connect(ui->btn_ajout_emp, &QPushButton::clicked, this, &SmartContentCreationStore::on_pushButton_ajouter_clicked);
    connect(ui->btn_chercher_emp, &QPushButton::clicked, this, &SmartContentCreationStore::on_pushButton_chercher_clicked);
    connect(ui->btn_modif_emp, &QPushButton::clicked, this, &SmartContentCreationStore::on_pushButton_modif_clicked);
    connect(ui->btn_supp_emp, &QPushButton::clicked, this, &SmartContentCreationStore::on_pushButton_supprimer_clicked);
    connect(ui->btn_tri_date, &QPushButton::clicked, this, &SmartContentCreationStore::on_btn_tri_date_clicked);

    connect(ui->btn_annuler_emp, &QPushButton::clicked, this, &SmartContentCreationStore::on_btn_clear_clicked);

    /*connect(ui->btn_faceid, &QPushButton::clicked, this, &SmartContentCreationStore::on_btn_faceid_clicked);*/
    connect(ui->btn_motdepasse, &QPushButton::clicked, this, &SmartContentCreationStore::on_btn_motdepasse_oublie_clicked);

    updateEmployeCountLabel();





    // ✅ Notification au démarrage
    verifierRappelsEvenements();

    // ➕ Charger le cache du chatbot
    chargerValeursUniquesEvenements();
    chargerConnaissances();
    chargerMemoire();

    // ➕ Connexion du btn envoyer du chatbot
    connect(ui->btnchatbot, &QPushButton::clicked, this, [=]() {
        QString question = ui->textquestion->toPlainText().trimmed();
        if (question.isEmpty()) return;
        QString reponse = repondreQuestionEvenement(question);
        ui->txtconv->append("<b>Vous :</b> " + question);
        ui->txtconv->append("<b>Assistant :</b> " + reponse.replace("\n", "<br>"));
        ui->textquestion->clear();
    });
}
/*

//----------------------------------------FACE_ID-----------------------------------------------

void SmartContentCreationStore::on_btn_faceid_clicked() {
    // ---- SINGLE WINDOW LOGIC ----
    if (cameraWindow && cameraWindow->isVisible()) {
        cameraWindow->raise();
        cameraWindow->activateWindow();
        QMessageBox::information(this, "Caméra", "La fenêtre caméra est déjà ouverte !");
        return;
    }
    if (cameraWindow) {
        delete cameraWindow;
        cameraWindow = nullptr;
    }

    cameraWindow = new QWidget;
    cameraWindow->setAttribute(Qt::WA_DeleteOnClose);
    cameraWindow->setWindowTitle("Live Face Detection");
    cameraWindow->setMinimumSize(400, 340);

    QLabel* detectionLabel = new QLabel(cameraWindow);
    detectionLabel->setFixedSize(400, 300);
    detectionLabel->setAlignment(Qt::AlignCenter);
    detectionLabel->setStyleSheet("border: 2px solid #4A90E2; background: #f8f9fd;");
    detectionLabel->setText("Détection en cours…");

    QPushButton* btnRegisterFace = new QPushButton("Enregistrer le visage", cameraWindow);
    btnRegisterFace->setFixedWidth(170);

    QVBoxLayout* layout = new QVBoxLayout(cameraWindow);
    layout->addWidget(detectionLabel);
    layout->addWidget(btnRegisterFace, 0, Qt::AlignHCenter);
    cameraWindow->setLayout(layout);
    cameraWindow->show();

    // ---- Camera & Capture ----
    QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
    if (cameras.isEmpty()) {
        QMessageBox::warning(this, "Caméra", "Aucune caméra détectée !");
        delete cameraWindow; cameraWindow = nullptr;
        return;
    }
    if (camera) { delete camera; camera = nullptr; }
    if (captureSession) { delete captureSession; captureSession = nullptr; }
    if (imageCapture) { delete imageCapture; imageCapture = nullptr; }
    if (faceIdTimer) { delete faceIdTimer; faceIdTimer = nullptr; }

    camera = new QCamera(cameras[0]);
    captureSession = new QMediaCaptureSession;
    imageCapture = new QImageCapture;
    captureSession->setCamera(camera);
    captureSession->setImageCapture(imageCapture);
    camera->start();

    faceIdTimer = new QTimer(cameraWindow);
    connect(faceIdTimer, &QTimer::timeout, cameraWindow, [this]() {
        if (imageCapture && camera && camera->isActive()) {
            imageCapture->capture();
        }
    });
    faceIdTimer->start(10);

    // ---- Face detection state (no recognition) ----
    static QImage lastDetectedFace;

    connect(imageCapture, &QImageCapture::imageCaptured, cameraWindow,
            [this, detectionLabel](int, const QImage &img) {
                if (img.isNull()) return;

                QImage rgbImg = img.convertToFormat(QImage::Format_RGB888);
                cv::Mat frame(rgbImg.height(), rgbImg.width(), CV_8UC3, const_cast<uchar*>(rgbImg.bits()), rgbImg.bytesPerLine());
                cv::Mat gray;
                cv::cvtColor(frame, gray, cv::COLOR_RGB2GRAY);

                static cv::CascadeClassifier faceCascade;
                static bool loaded = false;
                if (!loaded) {
                    loaded = faceCascade.load("haarcascade_frontalface_default.xml");
                    if (!loaded) return;
                }
                std::vectorv::Rect> faces;
                faceCascade.detectMultiScale(gray, faces, 1.1, 3);

                // NO recognition, just detection rectangles
                for (const auto& face : faces)
                    cv::rectangle(frame, face, cv::Scalar(0, 255, 0), 2);

                // Optionally save last face for registering
                if (!faces.empty()) {
                    cv::Mat faceROI = gray(faces[0]);
                    QImage faceImage(faceROI.data, faceROI.cols, faceROI.rows, faceROI.step, QImage::Format_Grayscale8);
                    lastDetectedFace = faceImage.copy();
                }

                QImage showImg(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
                detectionLabel->setPixmap(QPixmap::fromImage(showImg));
                detectionLabel->setText("");
            });

    connect(btnRegisterFace, &QPushButton::clicked, cameraWindow, [this]() {
        if (lastDetectedFace.isNull()) {
            QMessageBox::warning(cameraWindow, "Face", "Aucun visage détecté à enregistrer !");
            return;
        }
        QString saveName = QFileDialog::getSaveFileName(cameraWindow, "Enregistrer le visage", "", "Images (*.png)");
        if (saveName.isEmpty()) return;
        lastDetectedFace.save(saveName);
        QMessage
*/


//--------------------------se connecter--------------------------------------
// SmartContentCreationStore.cpp - Fixed version

void SmartContentCreationStore::on_btn_connect_clicked() {
    QString typedLogin = ui->lelogin->text();
    QString typedPassword = ui->lepassword->text();

    QString typedPasswordHash = hashPassword(typedPassword);
    qDebug() << "Login field:" << typedLogin;
    qDebug() << "Password field:" << typedPassword;
    qDebug() << "Hashed password:" << typedPasswordHash;

    // Delegate query to employes class instance
    if (Etmp.btn_connect(typedLogin, typedPasswordHash)) {
        ui->stackedWidget_menu->setCurrentIndex(1); // Login success
    } else {
        QMessageBox::warning(this, "Erreur", "Login ou mot de passe incorrect !");
    }
}

//---------------------------nb_Employe----------------------------
void SmartContentCreationStore::updateEmployeCountLabel() {
    qDebug() << "Called updateEmployeCountLabel";
    int count = Etmp.getEmployeeCount();  // Delegate to employes class

    if (count >= 0) {
        qDebug() << "Result:" << count;
        ui->nbEmploys_label->setText(QString::number(count));
    } else {
        qDebug() << "Count error";
        ui->nbEmploys_label->setText("Erreur");
    }
}

void SmartContentCreationStore::on_btn_retour_menu_login_clicked(){
    ui->stackedWidget1->setCurrentIndex(0);
}


//---------------------------retour login-------------------------------------
void SmartContentCreationStore::on_btn_retour_menu_login(){

    ui->stackedWidget_menu->setCurrentIndex(0);
}
//------------------------------retour_menu----------------------------------
void SmartContentCreationStore::on_pushButton_retour_menu_clicked()
{
    qDebug() << "on_pushButton_retour_menu_emp_clicked() called";
    ui->stackedWidget1->setCurrentIndex(0);
}
//----------------------------ajouter_base------------------------------------
void SmartContentCreationStore::on_pushButton_ajouter_clicked(){

    // --- Contrôles de saisie et messages d'erreur ---
    if (ui->lineEdit_Id->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir un ID valide (entier positif).");
        return;
    }
    if (ui->lineEdit_prenom->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir un prénom (lettres uniquement).");
        return;
    }
    if (ui->lineEdit_nom->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir un nom (lettres uniquement).");
        return;
    }
    if (ui->lineEdit_motpasse->text().isEmpty() || ui->lineEdit_motpasse->text().length() < 4) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir un mot de passe (au moins 4 caractères).");
        return;
    }
    if (ui->lineEdit_email->text().isEmpty() || !ui->lineEdit_email->hasAcceptableInput()) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir un email valide.");
        return;
    }
    if (ui->lineEdit_role->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir un rôle (lettres uniquement).");
        return;
    }
    if (ui->lineEdit_salaire->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir un salaire valide (nombre positif).");
        return;
    }
    // Vérifie aussi la date si tu veux :
    if (!ui->date->date().isValid()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une date valide.");
        return;
    }
    if (ui->lineEdit_questionrep->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez compléter le champ question/réponse.");
        return;
    }

    //Récupération des informations saisies dans les 3 champs
    int id = ui->lineEdit_Id->text().toInt();
    QString nom = ui->lineEdit_nom->text();
    QString prenom = ui->lineEdit_prenom->text();
    QString email=ui->lineEdit_email->text();
    QString mot_passe=hashPassword(ui->lineEdit_motpasse->text()); // ✅ FIXED: HASH PASSWORD
    float salaire=ui->lineEdit_salaire->text().toFloat();
    QDate date_rec = ui->date->date();
    QString role=ui->lineEdit_role->text();
    QString question_rep=ui->lineEdit_questionrep->text();

    // Debug: Check if values are read correctly
    qDebug() << "=== Values from UI ===";
    qDebug() << "ID:" << id;
    qDebug() << "Nom:" << nom;
    qDebug() << "Prenom:" << prenom;
    qDebug() << "Email:" << email;
    qDebug() << "Mot_passe (HASHED):" << mot_passe;
    qDebug() << "Salaire:" << salaire;
    qDebug() << "Date_rec:" << date_rec.toString("yyyy-MM-dd");
    qDebug() << "Role:" << role;
    qDebug() << "Question_rep:" << question_rep;

    employes e(id,nom,prenom,email,mot_passe, salaire, date_rec, role, question_rep);

    bool test = e.ajouter();

    if(test) {
        //refresh tableview
        ui->tableView->setModel(Etmp.afficher());
        ui->tableView->resizeColumnsToContents();
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Ajout effectué \n"
                                             "Click Cancel to exit."), QMessageBox::Cancel);
    }
    else {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Ajout non effectué.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
    }

    ui->tableView->setModel(Etmp.afficher());
    showEmployeeRoleChart(); // Refresh chart after update!
}

//---------------------------------chercher------------------------------------

void SmartContentCreationStore::on_pushButton_chercher_clicked() {
    int id = ui->lineEdit_ID->text().toInt();

    // Use the new model for search
    QSqlQueryModel* model = Etmp.affichercherch(id);
    ui->tableView->setModel(model);

    if (model->rowCount() > 0) {
        QMessageBox::information(this, tr("Trouvé"), tr("Employé trouvé !"));
        // (Optional) update stats/chart if you want based on this result:
        showEmployeeRoleChart();
    } else {
        QMessageBox::critical(this, tr("Non trouvé"), tr("Aucun employé avec cet ID !"));
        // Optionally clear the table view or restore full data:
        // ui->tableView->setModel(Etmp.afficher());
    }
    showEmployeeRoleChart();
}


//----------------------------Modifier_base------------------------------------
void SmartContentCreationStore::on_pushButton_modif_clicked(){
    int id = ui->lineEdit_Id->text().toInt();
    QString nom = ui->lineEdit_nom->text();
    QString prenom = ui->lineEdit_prenom->text();
    QString email=ui->lineEdit_email->text();
    QString mot_passe=hashPassword(ui->lineEdit_motpasse->text()); // ✅ FIXED: HASH PASSWORD
    float salaire=ui->lineEdit_salaire->text().toFloat();
    QDate date_rec = ui->date->date();
    QString role=ui->lineEdit_role->text();
    QString question_rep=ui->lineEdit_questionrep->text();

    // ** Create updated employe object **
    employes e(id, nom, prenom, email, mot_passe, salaire, date_rec, role, question_rep);
    bool success = e.modifier(id);

    if (success) {
        //refresh tableview
        ui->tableView->setModel(Etmp.afficher());
        ui->tableView->resizeColumnsToContents();
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        QMessageBox::information(this, tr("OK"), tr("Modification effectuée !"));
    } else {
        QMessageBox::critical(this, tr("Erreur"), tr("Modification impossible !!"));
    }

    ui->tableView->setModel(Etmp.afficher());
    showEmployeeRoleChart(); // Refresh chart after update!
}

//------------------------supprimer---------------------------------------
void SmartContentCreationStore::on_pushButton_supprimer_clicked()
{
    int id = ui->lineEdit_ID->text().toInt();
    bool test = Etmp.supprimer(id);

    if(test)
    {
        //refresh tableview
        ui->tableView->setModel(Etmp.afficher());
        ui->tableView->resizeColumnsToContents();
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Suppression effectuée\nClick Cancel to exit."),
                                 QMessageBox::Cancel);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Suppression non effectuée.\nClick Cancel to exit."),
                              QMessageBox::Cancel);
    }

    ui->tableView->setModel(Etmp.afficher());
    showEmployeeRoleChart(); // Refresh chart after update!
}

//----------------clear----------------
void SmartContentCreationStore::on_btn_clear_clicked()
{
    // Clear text fields
    ui->lineEdit_Id->clear();
    ui->lineEdit_nom->clear();
    ui->lineEdit_prenom->clear();
    ui->lineEdit_email->clear();
    ui->lineEdit_motpasse->clear();
    ui->lineEdit_salaire->clear();
    ui->lineEdit_role->clear();
    ui->lineEdit_questionrep->clear();

    // Reset date edit to today
    ui->date->setDate(QDate::currentDate());

    // If you have other widgets, clear/reset them here
}

// button action return of ajouter/modifier
void SmartContentCreationStore::on_btnretour_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(0);
}

//------------------tri_date---------------------------
void SmartContentCreationStore::on_btn_tri_date_clicked()
{
    QSqlQueryModel* model = Etmp.afficher_tri_date();
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->hide();

    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // Debug sorted dates:
    for (int row = 0; row < model->rowCount(); ++row) {
        QVariant v = model->data(model->index(row, 6));
        qDebug() << "Sorted RECRUTEMENT:" << v.toString();
    }
    QMessageBox::information(this, "Tri effectué", "Les employés ont été triés par date.");
}

void SmartContentCreationStore::on_btn_motdepasse_oublie_clicked()
{
    QString login = ui->lelogin->text();
    if (login.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Entrez d'abord votre login (prénom nom)");
        return;
    }

    // 1) Récupérer question + réponse depuis EMPLOYEE via employes (emp)
    QString questionTexte;
    QString reponseStockee;

    if (!Etmp.getQuestionEtReponse(login, questionTexte, reponseStockee)) {
        QMessageBox::warning(this, "Erreur", "Login inconnu ou QUESTION_REP invalide");
        return;
    }

    // 2) Demander la réponse utilisateur
    bool ok;
    QString reponseUser = QInputDialog::getText(this,
                                                "Question de sécurité",
                                                questionTexte + "\n\nVotre réponse :",
                                                QLineEdit::Normal,
                                                "",
                                                &ok);
    if (!ok || reponseUser.isEmpty()) {
        // L'utilisateur a annulé ou rien saisi
        return;
    }

    if (reponseUser.trimmed().toLower() != reponseStockee.trimmed().toLower()) {
        QMessageBox::warning(this, "Erreur", "Réponse incorrecte");
        return;
    }

    // 3) Saisie du nouveau mot de passe + confirmation
    QString nouveauMDP;
    while (true) {
        bool ok1;
        nouveauMDP = QInputDialog::getText(this,
                                           "Nouveau mot de passe",
                                           "Entrez votre nouveau mot de passe (min 8 caractères) :",
                                           QLineEdit::Password,
                                           "",
                                           &ok1);
        if (!ok1) {
            // Annulé
            return;
        }

        if (nouveauMDP.length() < 8) {
            if (QMessageBox::question(this,
                                      "Trop court",
                                      "Mot de passe trop court (< 8). Réessayer ?")
                == QMessageBox::No) {
                return;
            }
            continue;
        }

        bool ok2;
        QString confirmation = QInputDialog::getText(this,
                                                     "Confirmation",
                                                     "Confirmez le nouveau mot de passe :",
                                                     QLineEdit::Password,
                                                     "",
                                                     &ok2);
        if (!ok2) {
            // Annulé
            return;
        }

        if (nouveauMDP != confirmation) {
            if (QMessageBox::question(this,
                                      "Erreur",
                                      "Les mots de passe ne correspondent pas. Réessayer ?")
                == QMessageBox::No) {
                return;
            }
        } else {
            // OK, mot de passe valide et confirmé
            break;
        }
    }

    // 4) HASHAGE + mise à jour dans EMPLOYEE.MOT_DE_PASSE via emp
    QString nouveauHash = hashPassword(nouveauMDP);  // SHA-256 -> hex

    if (Etmp.mettreAJourMotDePasse(login, nouveauHash)) {
        QMessageBox::information(this,
                                 "Succès",
                                 "Mot de passe mis à jour avec succès.\nVous pouvez maintenant vous connecter.");
        ui->lepassword->clear();
        ui->lelogin->clear();
    } else {
        QMessageBox::critical(this,
                              "Erreur",
                              "Échec de la mise à jour du mot de passe.");
    }
}

//------------------------------static_emp-----------------------------------------
void SmartContentCreationStore::showEmployeeRoleChart()
{
    QAbstractItemModel* model = ui->tableView->model();
    if (!model) {
        QMessageBox::warning(this, "Erreur", "Le modèle n'est pas disponible.");
        return;
    }

    int rowCount = model->rowCount();
    if (rowCount == 0) {
        QMessageBox::information(this, "Aucun employé (statique tab)", "Aucun employé à afficher.");
        return;
    }

    // ID=0, Nom=1, Prenom=2, Mdp=3, Email=4, Salaire=5, date=6, role=7, question=8
    int colRole = 7;

    QMap<QString,int> roleCounts;
    int maxCount = 0;

    for (int i = 0; i < rowCount; ++i) {
        QString role = model->index(i, colRole).data().toString().trimmed();

        // Normaliser seulement la faute "roll" -> "role"
        if (role.compare("roll", Qt::CaseInsensitive) == 0)
            role = "role";

        if (role.isEmpty())
            continue;

        int newCount = ++roleCounts[role];
        if (newCount > maxCount)
            maxCount = newCount;
    }

    if (roleCounts.isEmpty()) {
        QMessageBox::information(this, "Aucun rôle", "Aucun rôle à afficher.");
        return;
    }

    QBarSeries *series = new QBarSeries();
    QBarSet *rolesSet = new QBarSet("Nombre d'employés");
    QStringList categories;

    for (auto it = roleCounts.constBegin(); it != roleCounts.constEnd(); ++it) {
        categories << it.key();      // "role", "Admin", "prof", etc.
        *rolesSet << it.value();     // compte par rôle
    }

    rolesSet->setColor(QColor("#4A90E2"));
    rolesSet->setLabelColor(Qt::white);
    series->append(rolesSet);
    series->setLabelsVisible(true);
    series->setLabelsFormat("@value");
    series->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statistiques des Rôles des Employés");
    chart->setTitleFont(QFont("Arial", 18, QFont::Bold));
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(false);
    chart->setBackgroundBrush(QBrush(QColor("#f8f9fd")));

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setTitleText("Rôles");
    axisX->setLabelsAngle(0);
    axisX->setLabelsFont(QFont("Arial", 12, QFont::Bold));
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Nombre d'employés");
    axisY->setLabelsFont(QFont("Arial", 11));
    axisY->setLabelFormat("%d");      // integers only
    axisY->setTickType(QValueAxis::TicksDynamic);
    axisY->setTickInterval(1);        // step = 1
    axisY->setMinorTickCount(0);

    // maxCount is max number of employees in one role
    axisY->setRange(0, qMax(3, maxCount + 1));   // e.g. 0..3 if max is 2

    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    if (auto oldLayout = ui->widgetChart->layout()) {
        QLayoutItem *item;
        while ((item = oldLayout->takeAt(0))) {
            if (auto w = item->widget()) w->deleteLater();
            delete item;
        }
        delete oldLayout;
    }

    QChartView *chartView = new QChartView(chart, ui->widgetChart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumHeight(320);
    chartView->setStyleSheet("background: transparent; border: none;");

    auto layout = new QVBoxLayout(ui->widgetChart);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->addWidget(chartView);
    ui->widgetChart->setLayout(layout);
}

// ===================================================================================
// ====== FONCTIONS ÉVÉNEMENTS (inchangées) ==========================================
// ===================================================================================
// ---------------------- Helper: Mise à jour des stats événements ----------------------
void SmartContentCreationStore::updateEventStats()
{
    Evenement::StatistiquesParType(ui->chatviewevent);
}

void SmartContentCreationStore::refreshEventTable() {
    ui->tableWidgetevent->blockSignals(true);
    Eevent.chargerDonnees(ui->tableWidgetevent);
    ui->tableWidgetevent->blockSignals(false);
    m_currentSelectedEventID = -1;
    m_editingEventId = -1;
    ui->btn_ajout->setText("Ajouter");
    updateEventStats();
}

void SmartContentCreationStore::fillEventFormFromRow(int row)
{
    if (row < 0 || row >= ui->tableWidgetevent->rowCount()) return;
    QTableWidgetItem *idItem = ui->tableWidgetevent->item(row, 0);
    QTableWidgetItem *nomItem = ui->tableWidgetevent->item(row, 1);
    QTableWidgetItem *typeItem = ui->tableWidgetevent->item(row, 2);
    QTableWidgetItem *debutItem = ui->tableWidgetevent->item(row, 3);
    QTableWidgetItem *finItem = ui->tableWidgetevent->item(row, 4);
    QTableWidgetItem *lieuItem = ui->tableWidgetevent->item(row, 5);
    if (!idItem || !nomItem || !typeItem || !debutItem || !finItem || !lieuItem)
        return;
    m_editingEventId = idItem->text().toInt();
    ui->lineEditeventnom_5->setText(nomItem->text());
    ui->comboeventtrierBox_2->setCurrentText(typeItem->text());
    ui->dateEditeventdebut->setDate(QDate::fromString(debutItem->text(), "yyyy-MM-dd"));
    ui->dateEditeventfin->setDate(QDate::fromString(finItem->text(), "yyyy-MM-dd"));
    ui->lineEditeventlieu_9->setText(lieuItem->text());
    ui->btn_ajout->setText("Enregistrer les modifications");
}

void SmartContentCreationStore::on_pushButtoneventtrier_9_clicked()
{
    QString typeChoisi = ui->comboBoxtrierevent->currentText().trimmed();
    if (typeChoisi.isEmpty() || typeChoisi == "Tous") {
        refreshEventTable();
        return;
    }
    Evenement::chargerDonneesTrieParType(ui->tableWidgetevent, typeChoisi);
    updateEventStats();
}

void SmartContentCreationStore::on_btn_ajout_clicked() {
    QString nom = ui->lineEditeventnom_5->text().simplified();
    QString type = ui->comboeventtrierBox_2->currentText();
    QString debut = ui->dateEditeventdebut->date().toString("yyyy-MM-dd");
    QString fin = ui->dateEditeventfin->date().toString("yyyy-MM-dd");
    QString lieu = ui->lineEditeventlieu_9->text().simplified();

    if(nom.isEmpty() || type.isEmpty() || debut.isEmpty() || fin.isEmpty() || lieu.isEmpty()){
        QMessageBox::critical(this, "Erreur", "Tous les champs sont requis !");
        return;
    }

    if (ui->dateEditeventfin->date() < ui->dateEditeventdebut->date()) {
        QMessageBox::warning(this, "Erreur", "La date de fin ne peut pas être antérieure à la date de début.");
        return;
    }

    if (m_editingEventId != -1) {
        if (Evenement::modifier(m_editingEventId, nom, type, debut, fin, lieu)) {
            QMessageBox::information(this, "Succès", "Événement modifié !");
            refreshEventTable();
            verifierRappelsEvenements();
            chargerValeursUniquesEvenements();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la modification.");
        }
    } else {
        Evenement ev(nom, type, debut, fin, lieu);
        if(ev.ajouter()){
            QMessageBox::information(this, "Succès", "Événement ajouté !");
            refreshEventTable();
            verifierRappelsEvenements();
            chargerValeursUniquesEvenements();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de l'ajout.");
        }
    }
}

void SmartContentCreationStore::on_btn_supprimer_clicked() {
    int row = ui->tableWidgetevent->currentRow();
    if(row < 0){ QMessageBox::warning(this,"Erreur","Sélectionnez un événement."); return; }
    QTableWidgetItem *item = ui->tableWidgetevent->item(row,0);
    if(!item){ QMessageBox::critical(this,"Erreur","Impossible de lire l'ID."); return; }

    int id = item->text().toInt();
    if(QMessageBox::question(this,"Confirmer","Supprimer cet événement ?")==QMessageBox::No) return;

    if(Evenement::supprimer(id)){
        QMessageBox::information(this,"Succès","Événement supprimé !");
        refreshEventTable();
        verifierRappelsEvenements();
        chargerValeursUniquesEvenements();
    } else {
        QMessageBox::critical(this,"Erreur","Échec de la suppression.");
    }
}

void SmartContentCreationStore::on_tableWidgetevent_itemSelectionChanged() {
    QList<QTableWidgetSelectionRange> selection = ui->tableWidgetevent->selectedRanges();
    bool hasSelection = !selection.isEmpty();
    ui->btn_modifier->setEnabled(hasSelection);

    if(hasSelection){
        int row = selection.first().topRow();
        QTableWidgetItem *idItem = ui->tableWidgetevent->item(row,0);
        if(idItem) m_currentSelectedEventID = idItem->text().toInt();
    } else {
        m_currentSelectedEventID = -1;
    }
}

void SmartContentCreationStore::on_btn_modifier_clicked() {
    int row = ui->tableWidgetevent->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Erreur", "Sélectionnez un événement.");
        return;
    }
    fillEventFormFromRow(row);
    ui->stackedWidgetemp->setCurrentIndex(1);
}

void SmartContentCreationStore::on_pushButtoneventrech_12_clicked()
{
    QString texteRecherche = ui->lineEditeventrech_7->text();

    if (texteRecherche.isEmpty()) {
        // 🔁 Recharger TOUTES les données (état initial)
        refreshEventTable();
    } else {
        // 🔍 Faire la recherche
        Evenement::rechercherParNomSQL(ui->tableWidgetevent, texteRecherche);
    }

    updateEventStats(); // toujours à la fin
}

void SmartContentCreationStore::on_pushButtoneventpdf_2_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("Exporter tous les événements en PDF"),
        QDir::homePath() + "/Evenements.pdf",
        tr("Fichiers PDF (*.pdf)")
        );
    if (fileName.isEmpty()) return;
    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive))
        fileName += ".pdf";
    if (Evenement::exporterTousEvenementsEnPDF(fileName)) {
        QMessageBox::information(this, tr("Succès"), tr("Export PDF terminé !"));
    } else {
        QMessageBox::critical(this, tr("Erreur"), tr("Échec de l'export."));
    }
}

// 🔹 Fonction de rappel SANS IA
void SmartContentCreationStore::verifierRappelsEvenements()
{
    if (!ui || !ui->textNotif) {
        return;
    }

    auto evenementsImminents = Evenement::recupererEvenementsImminents(3);
    QStringList messages;

    QDate aujourdhui = QDate::currentDate();
    for (const auto &ev : evenementsImminents) {
        QString nom = ev.first;
        QDate debut = ev.second;
        int joursRestants = aujourdhui.daysTo(debut);
        if (joursRestants == 0) {
            messages << QString("🟢 <b>%1</b> commence <b>AUJOURD'HUI</b> !").arg(nom);
        } else {
            messages << QString("📅 <b>%1</b> commence dans <b>%2 jour(s)</b>.").arg(nom).arg(joursRestants);
        }
    }

    if (!messages.isEmpty()) {
        QString fullMessage = "<b>📢 Événements à venir :</b><br>" + messages.join("<br>");
        ui->textNotif->setHtml(fullMessage);
        ui->textNotif->setStyleSheet(
            "background-color: #fff3cd; "
            "color: #856404; "
            "border: 1px solid #ffeaa7; "
            "padding: 6px; "
            "font-family: Arial, sans-serif; "
            "font-size: 13px;"
            );
    } else {
        ui->textNotif->clear();
        ui->textNotif->setStyleSheet("");
    }
}

// ➕ CORRIGÉ : utiliser connexion explicite
void SmartContentCreationStore::chargerValeursUniquesEvenements()
{
    m_idsEvenements = Evenement::recupererTousIDs();
    m_nomsEvenements = Evenement::recupererTousNomsUniques();
    m_typesEvenements = Evenement::recupererTousTypesUniques();
    m_lieuxEvenements = Evenement::recupererTousLieuxUniques();

    qDebug() << "✅ Cache événements chargé :" << m_idsEvenements.size() << "événements, IDs:" << m_idsEvenements;
}

// ➕ Chargement des fichiers texte
void SmartContentCreationStore::chargerConnaissances()
{
    m_connaissances.clear();

    QString filePath = "C:/Users/Moi/Desktop/Projetcpp/sccst/build/Desktop_Qt_6_7_3_MinGW_64_bit-Debug/debug/data/connaissances.txt";

    qDebug() << "🔍 Chemin testé :" << filePath;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "❌ Erreur d'ouverture :" << file.errorString();
        return;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    // ... reste de ton code (lecture du contenu)
    QString currentKey;
    QStringList currentReponses;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.startsWith("[mot-clé:") && line.endsWith("]")) {
            if (!currentKey.isEmpty() && !currentReponses.isEmpty()) {
                m_connaissances[currentKey] = currentReponses;
            }
            currentKey = line.mid(10, line.size() - 11).trimmed();
            currentReponses.clear();
        } else if (!line.isEmpty()) {
            currentReponses.append(line);
        }
    }

    if (!currentKey.isEmpty() && !currentReponses.isEmpty()) {
        m_connaissances[currentKey] = currentReponses;
    }

    file.close();
    qDebug() << "✅ Chargement réussi. Nombre de mots-clés :" << m_connaissances.size();
}
void SmartContentCreationStore::chargerMemoire()
{
    m_memoire.clear();

    QString filePath = "C:/Users/Moi/Desktop/Projetcpp/sccst/build/Desktop_Qt_6_7_3_MinGW_64_bit-Debug/debug/data/memoire.txt";

    QFile file(filePath);

    if (!file.exists()) {
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            file.close();
            qDebug() << "📄 Fichier memoire.txt créé vide.";
        }
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "⚠️ Impossible d'ouvrir data/memoire.txt";
        qDebug() << "    Chemin utilisé :" << filePath;
        return;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    QString currentKey;
    QString currentReponse;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        if (line.startsWith("[mot-clé:") && line.endsWith("]")) {
            // Sauvegarder l'entrée précédente si elle existe
            if (!currentKey.isEmpty()) {
                m_memoire[currentKey] = currentReponse;
            }
            // Nouvelle clé
            currentKey = line.mid(10, line.size() - 11).trimmed();
            currentReponse.clear(); // ✅ Réinitialiser la réponse
        }
        else if (!line.isEmpty()) {
            if (!currentReponse.isEmpty()) currentReponse += " ";
            currentReponse += line;
        }
        // ⚠️ On ne sauvegarde PAS ici → on attend la prochaine clé ou la fin
    }

    // ✅ Sauvegarder la DERNIÈRE entrée après la boucle
    if (!currentKey.isEmpty()) {
        m_memoire[currentKey] = currentReponse;
    }

    file.close();
    qDebug() << "🧠 Mémoire chargée. Nombre d'entrées :" << m_memoire.size();
}
// ➕ CORRIGÉ : utiliser connexion explicite + TO_DATE pour Oracle
QString SmartContentCreationStore::repondreDepuisBase(const QString &question)
{
    QString lower = question.toLower();
    QDate today = QDate::currentDate();

    // 🔹 1. Recherche par ID
    QRegularExpression idRegex(R"(\b(?:événement|event|id)\s*(\d+))", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch idMatch = idRegex.match(question);
    if (idMatch.hasMatch()) {
        int id = idMatch.captured(1).toInt();
        if (m_idsEvenements.contains(id)) {
            return Evenement::repondreParID(id);
        }
        return QString("Aucun événement trouvé avec l'ID %1.").arg(id);
    }

    // 🔹 2. Recherche par nom
    for (const QString &nom : m_nomsEvenements) {
        if (lower.contains(nom.toLower())) {
            return Evenement::repondreParNom(nom);
        }
    }

    // 🔹 3. Recherche par type
    for (const QString &type : m_typesEvenements) {
        if (lower.contains(type.toLower())) {
            return Evenement::repondreParType(type);
        }
    }

    // 🔹 4. Recherche par lieu
    for (const QString &lieu : m_lieuxEvenements) {
        if (lower.contains(lieu.toLower())) {
            return Evenement::repondreParLieu(lieu);
        }
    }

    // 🔹 5. Recherche par date
    QRegularExpression dateRegex(R"(\b(\d{4}-\d{2}-\d{2})\b)");
    QRegularExpressionMatchIterator dateMatch = dateRegex.globalMatch(question);
    if (dateMatch.hasNext()) {
        QString dateStr = dateMatch.next().captured(1);
        QDate date = QDate::fromString(dateStr, "yyyy-MM-dd");
        if (date.isValid()) {
            return Evenement::repondreParDate(date);
        }
    }

    // 🔹 6. Questions génériques
    if (lower.contains("combien") && lower.contains("événement")) {
        int count = Evenement::compterTousEvenements();
        return QString("Il y a actuellement %1 événement(s) dans la base.").arg(count);
    }

    if (lower.contains("liste") && lower.contains("événement")) {
        return Evenement::listerTousEvenements();
    }

    if (lower.contains("à venir") || lower.contains("futur")) {
        return Evenement::listerEvenementsAFutur(today);
    }

    if (lower.contains("aujourd'hui") || lower.contains("aujourd’hui")) {
        return Evenement::listerEvenementsAujourdhui(today);
    }

    return QString();
}
// ➕ Réponse depuis fichiers texte
QString SmartContentCreationStore::repondreDepuisFichiers(const QString &question)
{
    QString lower = question.toLower().trimmed();
    if (lower.endsWith('?')) lower.chop(1);
    lower = lower.trimmed();

    // 🔹 1. Correspondance EXACTE dans la mémoire apprise
    if (m_memoire.contains(lower)) {
        return m_memoire.value(lower);
    }

    // 🔹 2. Correspondance EXACTE dans les connaissances fixes
    if (m_connaissances.contains(lower)) {
        const QStringList &reponses = m_connaissances.value(lower);
        if (!reponses.isEmpty()) {
            int index = QRandomGenerator::global()->bounded(reponses.size());
            return reponses[index];
        }
    }

    // 🔹 3. (Optionnel) Recherche partielle comme fallback
    for (auto it = m_memoire.begin(); it != m_memoire.end(); ++it) {
        if (lower == it.key().toLower() || lower.contains(it.key().toLower())) {
            return it.value();
        }
    }

    for (auto it = m_connaissances.begin(); it != m_connaissances.end(); ++it) {
        if (lower == it.key().toLower() || lower.contains(it.key().toLower())) {
            const QStringList &reponses = it.value();
            if (!reponses.isEmpty()) {
                int index = QRandomGenerator::global()->bounded(reponses.size());
                return reponses[index];
            }
        }
    }

    return QString();
}

// ➕ Apprentissage
void SmartContentCreationStore::apprendreNouvelleReponse(const QString &question, const QString &reponse)
{
    QString motCle = question;
    motCle.replace(QRegularExpression(R"((apprends|souviens-toi|souviens toi)\s+que\s+)", QRegularExpression::CaseInsensitiveOption), "");
    motCle = motCle.trimmed();
    if (motCle.endsWith('?')) motCle.chop(1);
    motCle = motCle.trimmed();
    if (motCle.isEmpty()) motCle = "inconnu";

    m_memoire[motCle] = reponse;

    QString filePath = "C:/Users/Moi/Desktop/Projetcpp/sccst/build/Desktop_Qt_6_7_3_MinGW_64_bit-Debug/debug/data/memoire.txt";

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out.setEncoding(QStringConverter::Utf8);
        out << "\n[mot-clé:" << motCle << "]\n";
        out << reponse << "\n";
        file.close();
        qDebug() << "✅ Nouvelle connaissance sauvegardée.";
    } else {
        qDebug() << "❌ Échec d'écriture dans :" << filePath;
        qDebug() << "    Erreur :" << file.errorString();
    }
}

// ➕ Fonction de réponse du chatbot
QString SmartContentCreationStore::repondreQuestionEvenement(const QString &question)
{
    QString lower = question.toLower();
    if (lower.startsWith("apprends que") || lower.startsWith("souviens-toi que") || lower.startsWith("souviens toi que")) {
        QString clean = question;
        clean.replace(QRegularExpression(R"((Apprends|Souviens-toi|Souviens toi)\s+que\s*)", QRegularExpression::CaseInsensitiveOption), "");
        clean = clean.trimmed();
        if (clean.contains('=')) {
            QStringList parts = clean.split('=', Qt::SkipEmptyParts);
            if (parts.size() >= 2) {
                QString q = parts[0].trimmed();
                QString r = parts[1].trimmed();
                apprendreNouvelleReponse(q, r);
                return "✅ J’ai bien enregistré : \"" + q + "\" = \"" + r + "\"";
            }
        }
        apprendreNouvelleReponse(clean, "Information apprise par l'utilisateur.");
        return "✅ J’ai enregistré cette information !";
    }

    QString reponseDB = repondreDepuisBase(question);
    if (!reponseDB.isEmpty()) {
        return reponseDB;
    }

    QString reponseFichiers = repondreDepuisFichiers(question);
    if (!reponseFichiers.isEmpty()) {
        return reponseFichiers;
    }

    return "Je ne sais pas. Vous pouvez m'apprendre quelque chose : dites « Apprends que [question] = [réponse] ».";
}


// ---------------------- Destructor ----------------------
SmartContentCreationStore::~SmartContentCreationStore() {
    delete ui;
}




// ====== GESTION PROJET ======
// ====== UTIL ======
template<typename T>
static T* need(QWidget* root, const char* name, bool fatal = true) {
    if (!root) return nullptr;
    auto* w = root->findChild<T*>(QString::fromUtf8(name));
    if (!w && fatal) {
        qFatal("Widget '%s' introuvable (verifie l'objectName dans le .ui).", name);
    }
    return w;
}

// ====== INITIALISATION ======
void SmartContentCreationStore::initGestionProjet()
{
    // === Rattacher les pointeurs aux widgets du .ui ===
    gp_table_    = ui->tableViewRight_2;
    gp_btnAdd_   = ui->ajoutgprojets_2;
    gp_btnEdit_  = ui->modifierprojet_3;
    gp_btnDel_   = ui->supprimerproj_3;
    gp_leSearch_ = ui->chercherprojet_2;

    // --- Champs du formulaire (adapte si les noms diffèrent) ---
    gp_leNom_ = ui->nom_3;
    gp_leDesc_   = ui->description_3; // ou ui->lineEdit_3 si c’est son nom exact
    gp_deDebut_  = ui->datedebutprojet_2;   // QDateEdit Date début
    gp_deFin_    = ui->datefinprojet_2;     // QDateEdit Date fin
    gp_cbStatut = ui->statutprojet_3;
    // QLineEdit Statut

    // === Vérif existence des boutons (crash clair si absent) ===
    Q_ASSERT_X(gp_table_, "initGestionProjet", "tableViewRight_2 introuvable");
    Q_ASSERT_X(gp_btnAdd_, "initGestionProjet", "poserprojet_4 introuvable");
    Q_ASSERT_X(gp_btnEdit_, "initGestionProjet", "modifierprojet_3 introuvable");
    Q_ASSERT_X(gp_btnDel_, "initGestionProjet", "supprimerproj_3 introuvable");
    Q_ASSERT_X(gp_leSearch_, "initGestionProjet", "chercherprojet_2 introuvable");

    // === Modèle/table ===
    if (!gp_model_) gp_model_ = new QSqlQueryModel(this);
    gp_table_->setModel(gp_model_);
    gp_table_->horizontalHeader()->setStretchLastSection(true);
    gp_table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    gp_table_->setSelectionMode(QAbstractItemView::SingleSelection);

    // === Connexions ===
    connect(gp_leSearch_, &QLineEdit::textChanged,
            this, &SmartContentCreationStore::onSearchProjet, Qt::UniqueConnection);
    connect(gp_btnAdd_,  &QPushButton::clicked,
            this, &SmartContentCreationStore::onAjouterProjet, Qt::UniqueConnection);
    connect(gp_btnEdit_, &QPushButton::clicked,
            this, &SmartContentCreationStore::onModifierProjet, Qt::UniqueConnection);
    connect(gp_btnDel_,  &QPushButton::clicked,
            this, &SmartContentCreationStore::onSupprimerProjet, Qt::UniqueConnection);
    connect(gp_table_->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &SmartContentCreationStore::loadProjetToForm, Qt::UniqueConnection);
    connect(ui->export_2gprojets_2, &QPushButton::clicked,
            this, &SmartContentCreationStore::on_export_2gprojets_2_clicked);



    refreshProjetTable();
    afficherStatistiques();
}

void SmartContentCreationStore::afficherStatistiques() {
    if (!gp_model_ || gp_model_->rowCount() == 0 || !ui || !ui->statprojet) return;

    QMap<QString, int> compteur;
    int total = gp_model_->rowCount();
    for (int row = 0; row < total; ++row) {
        QString statut = gp_model_->index(row, 5).data().toString();
        compteur[statut]++;
    }

    // === Créer la série en camembert ===
    QPieSeries *series = new QPieSeries();
    for (auto it = compteur.begin(); it != compteur.end(); ++it) {
        double pourcentage = total > 0 ? (it.value() * 100.0 / total) : 0;
        QPieSlice *slice = series->append(it.key(), it.value());
        slice->setLabel(QString("%1 (%2%)").arg(it.key()).arg(QString::number(pourcentage, 'f', 1)));
        slice->setLabelVisible(true);
    }
    // === Couleurs personnalisées (optionnel) ===
    // === Couleurs professionnelles et harmonisées ===
    for (QPieSlice *slice : series->slices()) {
        QString label = slice->label();

        if (label.contains("Avancé"))
            slice->setBrush(QColor("#1565C0")); // Bleu profond
        else if (label.contains("Initial"))
            slice->setBrush(QColor("#1E88E5")); // Bleu moyen
        else if (label.contains("Terminé"))
            slice->setBrush(QColor("#42A5F5")); // Bleu clair
        else if (label.contains("attente"))
            slice->setBrush(QColor("#90CAF9")); // Bleu pastel

        // Bordure blanche + effet classe
        slice->setPen(QPen(Qt::white, 1));
        slice->setExploded(true);
        slice->setExplodeDistanceFactor(0.05);
    }
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des projets par statut");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(400, 300);

    // === Affichage dans le widget statprojet ===
    QLayout *layout = ui->statprojet->layout();
    if (!layout) {
        layout = new QVBoxLayout(ui->statprojet);
        ui->statprojet->setLayout(layout);
    } else {
        QLayoutItem *item;
        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
    }

    layout->addWidget(chartView);
}


// ====== RAFRAÎCHIR LA TABLE ======
void SmartContentCreationStore::refreshProjetTable(const QString& nomLike) {
    QString sql =
        "SELECT ID_PROJET, NOM, DESCRIPTION, DATE_DEBUT, DATE_FIN, STATUT "
        "FROM CONTENTCREATION.PROJET";

    QSqlQuery q;
    if (!nomLike.trimmed().isEmpty()) {
        sql += " WHERE UPPER(NOM) LIKE UPPER(:s)";
        q.prepare(sql);
        q.bindValue(":s", "%" + nomLike.trimmed() + "%");
        q.exec();
    } else {
        q.exec(sql);
    }

    if (q.lastError().isValid()) {
        qWarning() << "[refreshProjetTable] SQL error:" << q.lastError();
        QMessageBox::warning(this, tr("Erreur SQL"), q.lastError().text());
        return;
    }

    gp_model_->setQuery(q);
    // En-têtes
    gp_model_->setHeaderData(0, Qt::Horizontal, tr("ID"));
    gp_model_->setHeaderData(1, Qt::Horizontal, tr("Nom"));
    gp_model_->setHeaderData(2, Qt::Horizontal, tr("Description"));
    gp_model_->setHeaderData(3, Qt::Horizontal, tr("Début"));
    gp_model_->setHeaderData(4, Qt::Horizontal, tr("Fin"));
    gp_model_->setHeaderData(5, Qt::Horizontal, tr("Statut"));
}
// ====== AIDE : récupérer l'ID sélectionné ======
static int selectedProjetId(QTableView* tv) {
    if (!tv || !tv->model()) return -1;

    QItemSelectionModel* sel = tv->selectionModel();
    if (!sel || sel->selectedIndexes().isEmpty()) return -1;

    QModelIndex selectedIndex = sel->selectedIndexes().first(); // index sélectionné
    int row = selectedIndex.row();

    QModelIndex idIndex = tv->model()->index(row, 0); // colonne 0 = ID_PROJET
    bool ok = false;
    int id = idIndex.data(Qt::DisplayRole).toInt(&ok);
    return ok ? id : -1;
}
// ====== AJOUTER ======
// ====== AJOUTER ======
void SmartContentCreationStore::onAjouterProjet() {
    const QString nom    = gp_leNom_    ? gp_leNom_->text().trimmed()    : QString();
    const QString desc   = gp_leDesc_   ? gp_leDesc_->text().trimmed()   : QString();
    const QString statut = gp_cbStatut ? gp_cbStatut->currentText().trimmed() : QString();
    const QDate   ddeb   = gp_deDebut_  ? gp_deDebut_->date()            : QDate();
    const QDate   dfin   = gp_deFin_    ? gp_deFin_->date()              : QDate();

    if (nom.isEmpty()) {
        QMessageBox::information(this, tr("Champs manquants"), tr("Le nom du projet est requis."));
        return;
    }

    // ✅ VALIDATION DES DATES (À AJOUTER)
    if (!ddeb.isValid() || !dfin.isValid()) {
        QMessageBox::warning(this, tr("Dates invalides"),
                             tr("Les deux dates doivent être correctement définies."));
        return;
    }

    if (ddeb >= dfin) {
        QMessageBox::warning(this, tr("Erreur de dates"),
                             tr("La date de début doit être strictement inférieure à la date de fin."));
        return;
    }

    QSqlQuery q;
    q.prepare("INSERT INTO CONTENTCREATION.PROJET (NOM, DESCRIPTION, DATE_DEBUT, DATE_FIN, STATUT) "
              "VALUES (:n, :d, :dd, :df, :s)");
    q.bindValue(":n", nom);
    q.bindValue(":d", desc);
    q.bindValue(":dd", ddeb);
    q.bindValue(":df", dfin);
    q.bindValue(":s", statut);

    if (!q.exec()) {
        QMessageBox::warning(this, tr("Échec d'ajout"), q.lastError().text());
        return;
    }
    refreshProjetTable(gp_leSearch_ ? gp_leSearch_->text() : QString());
    afficherStatistiques();   // ✅ ici pour mettre à jour après ajout

}

// ====== MODIFIER ======
void SmartContentCreationStore::onModifierProjet() {
    const int id = selectedProjetId(gp_table_);
    if (id < 0) {
        QMessageBox::information(this, tr("Sélection requise"),
                                 tr("Sélectionne d'abord une ligne dans la table."));
        return;
    }

    const QString nom    = gp_leNom_    ? gp_leNom_->text().trimmed()    : QString();
    const QString desc   = gp_leDesc_   ? gp_leDesc_->text().trimmed()   : QString();
    const QString statut = gp_cbStatut ? gp_cbStatut->currentText().trimmed() : QString();
    const QDate   ddeb   = gp_deDebut_  ? gp_deDebut_->date()            : QDate();
    const QDate   dfin   = gp_deFin_    ? gp_deFin_->date()              : QDate();

    // ================================
    // 📌 VALIDATION DES DATES
    // ================================
    if (!ddeb.isValid() || !dfin.isValid()) {
        QMessageBox::warning(this, tr("Dates invalides"),
                             tr("Les deux dates doivent être correctement définies."));
        return;
    }

    if (ddeb == dfin) {
        QMessageBox::warning(this, tr("Erreur de dates"),
                             tr("La date de début et la date de fin ne peuvent pas être égales."));
        return;
    }

    if (ddeb > dfin) {
        QMessageBox::warning(this, tr("Erreur de dates"),
                             tr("La date de début doit être strictement inférieure à la date de fin."));
        return;
    }
    // ================================
    if (nom.isEmpty()) {
        QMessageBox::information(this, tr("Champs manquants"), tr("Le nom du projet est requis."));
        return;
    }

    // ================================
    // 📌 MISE À JOUR SQL
    // ================================
    QSqlQuery q;
    q.prepare("UPDATE PROJET "
              "SET NOM=:n, DESCRIPTION=:d, DATE_DEBUT=:dd, DATE_FIN=:df, STATUT=:s "
              "WHERE ID_PROJET=:id");

    q.bindValue(":n", nom);
    q.bindValue(":d", desc);
    q.bindValue(":dd", ddeb);
    q.bindValue(":df", dfin);
    q.bindValue(":s", statut);
    q.bindValue(":id", id);

    if (!q.exec()) {
        QMessageBox::warning(this, tr("Échec de modification"), q.lastError().text());
        return;
    }

    // Rafraîchir la table
    refreshProjetTable(gp_leSearch_ ? gp_leSearch_->text() : QString());
    afficherStatistiques();
}
// ====== SUPPRIMER ======
void SmartContentCreationStore::onSupprimerProjet() {
    const int id = selectedProjetId(gp_table_);
    if (id < 0) {
        QMessageBox::information(this, tr("Sélection requise"),
                                 tr("Sélectionne d'abord une ligne dans la table."));
        return;
    }
    if (QMessageBox::question(this, tr("Confirmer"),
                              tr("Supprimer le projet %1 ?").arg(id))
        != QMessageBox::Yes) {
        return;
    }

    QSqlQuery q;
    q.prepare("DELETE FROM PROJET WHERE ID_PROJET = :id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        QMessageBox::warning(this, tr("Échec de suppression"), q.lastError().text());
        return;
    }

    initGestionProjet();
    afficherStatistiques();
}

// ====== RECHERCHE ======
void SmartContentCreationStore::onSearchProjet(const QString& text) {
    refreshProjetTable(text);
}

void SmartContentCreationStore::loadProjetToForm() {
    const int id = selectedProjetId(gp_table_);
    if (id < 0) {
        QMessageBox::information(this, tr("Sélection requise"),
                                 tr("Sélectionne une ligne dans la table des projets."));
        return;
    }

    QModelIndex index = gp_table_->selectionModel()->selectedRows(0).first();
    int row = index.row();

    if (gp_leNom_)    gp_leNom_->setText(gp_model_->index(row, 1).data().toString()); // NOM
    if (gp_leDesc_)   gp_leDesc_->setText(gp_model_->index(row, 2).data().toString()); // DESCRIPTION
    if (gp_deDebut_)  gp_deDebut_->setDate(QDate::fromString(gp_model_->index(row, 3).data().toString(), "yyyy-MM-dd")); // DATE_DEBUT
    if (gp_deFin_)    gp_deFin_->setDate(QDate::fromString(gp_model_->index(row, 4).data().toString(), "yyyy-MM-dd"));   // DATE_FIN
    if (gp_cbStatut) gp_cbStatut->setCurrentText(gp_model_->index(row, 5).data().toString());
}

bool SmartContentCreationStore::exporter(QTableView *tableView, const QString &title) {
    if (!tableView || !tableView->model()) {
        QMessageBox::warning(nullptr, "Erreur", "Le tableau ou son modèle est introuvable.");
        return false;
    }
    // Choix du fichier
    QString fileName = QFileDialog::getSaveFileName(
        nullptr, "Exporter en PDF", "", "PDF (*.pdf)");
    if (fileName.isEmpty()) return false;

    // PDF setup
    QPdfWriter pdf(fileName);
    pdf.setPageSize(QPageSize(QPageSize::A4));
    pdf.setResolution(300);

    QPainter painter(&pdf);
    if (!painter.isActive()) {
        QMessageBox::warning(nullptr, "Erreur", "Impossible d'initialiser le rendu PDF.");
        return false;
    }

    // Marges et métriques
    const int margin = 40; // px à 300 dpi ~ 0.13"
    QRect pageRect = QRect(margin, margin,
                           pdf.width() - 2 * margin,
                           pdf.height() - 2 * margin);

    // Polices
    QFont titleFont("Arial", 6, QFont::Bold);
    QFont headerFont("Arial", 6, QFont::Bold);
    QFont cellFont("Arial", 6);

    // Titre
    painter.setFont(titleFont);
    painter.drawText(pageRect, Qt::AlignTop | Qt::AlignHCenter, title.isEmpty() ? "Export de tableau" : title);

    // Avance sous le titre
    int y = pageRect.top() + painter.fontMetrics().height() + 20;

    // Modèle et entêtes
    QAbstractItemModel *model = tableView->model();
    const int rows = model->rowCount();
    const int cols = model->columnCount();

    // Largeurs de colonnes basées sur le QTableView
    QVector<int> colWidths(cols, 100);
    int totalWidth = 0;
    for (int c = 0; c < cols; ++c) {
        if (!tableView->isColumnHidden(c)) {
            int w = tableView->columnWidth(c);
            if (w <= 0) w = 100;
            colWidths[c] = w;
            totalWidth += w;
        }
    }
    if (totalWidth == 0) {
        QMessageBox::warning(nullptr, "Erreur", "Aucune colonne visible à exporter.");
        painter.end();
        return false;
    }

    // Mise à l'échelle pour tenir dans la largeur de la page
    const double scaleX = double(pageRect.width()) / double(totalWidth);
    painter.save();
    painter.translate(pageRect.left(), y);
    painter.scale(scaleX, 1.0);

    // Hauteurs
    const int headerHeight = 40;
    const int rowHeight = 40;

    // Dessin des en-têtes
    painter.setFont(headerFont);
    int x = 0;
    for (int c = 0; c < cols; ++c) {
        if (tableView->isColumnHidden(c)) continue;
        QRect cellRect(x, 0, colWidths[c], headerHeight);
        painter.drawRect(cellRect);
        QString headerText = model->headerData(c, Qt::Horizontal).toString();
        painter.drawText(cellRect.adjusted(4, 0, -4, 0), Qt::AlignVCenter | Qt::AlignLeft, headerText);
        x += colWidths[c];
    }

    int tableY = headerHeight;

    // Corps du tableau avec pagination
    painter.setFont(cellFont);
    for (int r = 0; r < rows; ++r) {
        // Si on dépasse la page en Y (hors scale), on saute à la page suivante
        if ((tableY + rowHeight) > (pageRect.height() - (y - pageRect.top()))) {
            painter.restore();      // annule scale et translate
            pdf.newPage();
            y = pageRect.top();     // nouveau haut de page
            painter.translate(pageRect.left(), y);
            painter.scale(scaleX, 1.0);

            // Redessine les en-têtes sur la nouvelle page
            painter.setFont(headerFont);
            x = 0;
            for (int c = 0; c < cols; ++c) {
                if (tableView->isColumnHidden(c)) continue;
                QRect cellRect(x, 0, colWidths[c], headerHeight);
                painter.drawRect(cellRect);
                QString headerText = model->headerData(c, Qt::Horizontal).toString();
                painter.drawText(cellRect.adjusted(4, 0, -4, 0), Qt::AlignVCenter | Qt::AlignLeft, headerText);
                x += colWidths[c];
            }
            painter.setFont(cellFont);
            tableY = headerHeight;
        }

        x = 0;
        for (int c = 0; c < cols; ++c) {
            if (tableView->isColumnHidden(c)) continue;
            QRect cellRect(x, tableY, colWidths[c], rowHeight);
            painter.drawRect(cellRect);

            QString data = model->data(model->index(r, c), Qt::DisplayRole).toString();
            painter.drawText(cellRect.adjusted(4, 0, -4, 0), Qt::AlignVCenter | Qt::AlignLeft, data);

            x += colWidths[c];
        }
        tableY += rowHeight;
    }

    // Fin du rendu
    painter.restore();
    painter.end();

    QMessageBox::information(nullptr, "Succès", "PDF exporté : " + fileName);
    return true;
}
void SmartContentCreationStore::on_export_2gprojets_2_clicked() {
    //QMessageBox::information(this, "Test", "Le bouton exporter a été cliqué !");
    exporter(ui->tableViewRight_2, "Liste des projets");
}
void SmartContentCreationStore::on_tridateproj_2_clicked()
{
    QString sql =
        "SELECT ID_PROJET, NOM, DESCRIPTION, DATE_DEBUT, DATE_FIN, STATUT "
        "FROM CONTENTCREATION.PROJET "
        "ORDER BY DATE_DEBUT ASC";

    QSqlQuery q;
    if (!q.exec(sql)) {
        qWarning() << "[triParDate] Erreur SQL:" << q.lastError().text();
        QMessageBox::warning(this, tr("Erreur de tri"),
                             tr("Impossible de trier les projets."));
        return;
    }
    gp_model_->setQuery(q);
    gp_model_->setHeaderData(0, Qt::Horizontal, tr("ID"));
    gp_model_->setHeaderData(1, Qt::Horizontal, tr("Nom"));
    gp_model_->setHeaderData(2, Qt::Horizontal, tr("Description"));
    gp_model_->setHeaderData(3, Qt::Horizontal, tr("Début"));
    gp_model_->setHeaderData(4, Qt::Horizontal, tr("Fin"));
    gp_model_->setHeaderData(5, Qt::Horizontal, tr("Statut"));

    // ✅ Message de confirmation
    QMessageBox::information(this, tr("Tri terminé"), tr("Les projets ont été triés avec succès."));
}
void SmartContentCreationStore::handleProjectRecommendation(const QString& question)
{
    // Extraire l'ID ou le nom du projet à partir de la question
    QRegularExpression idRegex(R"((?:id|identifiant|numéro)\s*[:\-]?\s*(\d+))", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression nameRegex(R"((?:nom|projet\s*de\s*)['\"]?([^'\"]+?)['\"]?(?=\s|$))", QRegularExpression::CaseInsensitiveOption);

    int projectId = -1;
    QString projectName;

    QRegularExpressionMatch matchId = idRegex.match(question);
    if (matchId.hasMatch()) {
        projectId = matchId.captured(1).toInt();
    }

    QRegularExpressionMatch matchName = nameRegex.match(question);
    if (matchName.hasMatch()) {
        projectName = matchName.captured(1).trimmed();
    }

    // Si ni ID ni nom trouvés → on ne peut pas faire de recommandation
    if (projectId == -1 && projectName.isEmpty()) {
        QString fallbackPrompt = "You are SCS.AI, assistant of Smart Content Creation Store.\n"
                                 "❌ Je n’ai pas pu identifier le projet demandé. Veuillez préciser :\n"
                                 "- L’ID du projet (ex: 'projet d’ID 25')\n"
                                 "- Ou son nom (ex: 'projet MOHAMED')\n\n"
                                 "Question : " + question;
        sendToGemma(question, fallbackPrompt, 0.4); // ✅ Fixed
        return;
    }

    // Requête pour récupérer les détails du projet
    QSqlQuery q;
    QString projectDetails;

    if (projectId > 0) {
        q.prepare("SELECT ID_PROJET, NOM, DESCRIPTION, DATE_DEBUT, DATE_FIN, STATUT FROM CONTENTCREATION.PROJET WHERE ID_PROJET = ?");
        q.addBindValue(projectId);
    } else if (!projectName.isEmpty()) {
        q.prepare("SELECT ID_PROJET, NOM, DESCRIPTION, DATE_DEBUT, DATE_FIN, STATUT FROM CONTENTCREATION.PROJET WHERE UPPER(NOM) LIKE UPPER(?)");
        q.addBindValue("%" + projectName + "%");
    }

    if (!q.exec() || !q.next()) {
        QString errorPrompt = "You are SCS.AI, assistant of Smart Content Creation Store.\n"
                              "❌ Aucun projet trouvé correspondant à votre demande.\n"
                              "Vérifiez l’ID ou le nom et réessayez.\n\n"
                              "Question : " + question;
        sendToGemma(question, errorPrompt, 0.4); // ✅ Also use 0.4 here
        return;
    }

    // Récupérer les données
    int id = q.value("ID_PROJET").toInt();
    QString nom = q.value("NOM").toString();
    QString desc = q.value("DESCRIPTION").toString();
    QString debut = q.value("DATE_DEBUT").toDate().toString("dd/MM/yyyy");
    QString fin = q.value("DATE_FIN").toDate().toString("dd/MM/yyyy");
    QString statut = q.value("STATUT").toString();

    // Calculer des indicateurs utiles
    QDate today = QDate::currentDate();
    QDate startDate = q.value("DATE_DEBUT").toDate();
    QDate endDate = q.value("DATE_FIN").toDate();
    bool isOverdue = (statut != "Terminé" && endDate < today);
    int daysLeft = endDate.daysTo(today); // négatif si en retard
    int durationDays = startDate.daysTo(endDate);

    // Construire le contexte pour Gemma
    QString context = QString(
                          "📊 Détails du projet demandé :\n"
                          "• ID_PROJET : %1\n"
                          "• Nom : %2\n"
                          "• Description : %3\n"
                          "• Date début : %4\n"
                          "• Date fin : %5\n"
                          "• Statut actuel : %6\n"
                          "• Durée totale : %7 jours\n"
                          "• En retard ? : %8\n"
                          "• Jours restants / dépassés : %9\n\n"
                          ).arg(id)
                          .arg(nom)
                          .arg(desc.left(100) + (desc.length() > 100 ? "..." : "")) // Tronquer si trop long
                          .arg(debut)
                          .arg(fin)
                          .arg(statut)
                          .arg(durationDays)
                          .arg(isOverdue ? "Oui" : "Non")
                          .arg(daysLeft);

    // Prompt pour Gemma — demande une recommandation
    QString systemPrompt =
        "Tu es SCS.AI, assistant intelligent de Smart Content Creation Store en Tunisie.\n"
        "Voici les détails d’un projet spécifique :\n\n"
        + context +
        "✅ Analyse ces données et donne UNE SEULE recommandation concrète, professionnelle et adaptée au contexte du projet.\n"
        "✅ Sois clair, bref, et utile. Réponds en français.\n"
        "❌ Ne jamais inventer des faits. Base-toi uniquement sur les données fournies.\n"
        "❌ Si le projet est terminé, propose un bilan ou une archivage.\n"
        "❌ Si le projet est en retard, propose un plan d’action.\n\n"
        "Recommandation : ";

    sendToGemma(question, systemPrompt, 0.4); // ✅ And here
}
void SmartContentCreationStore::handleProjectListRequest(const QString& question)
{
    QSqlQuery q;
    QStringList projects;
    if (q.exec("SELECT ID_PROJET, NOM FROM CONTENTCREATION.PROJET")) {
        while (q.next()) {
            projects << QString("• ID %1 : %2")
                            .arg(q.value("ID_PROJET").toString())
                            .arg(q.value("NOM").toString());
        }
    }

    QString prompt = "You are SCS.AI...\n"
                     "Here is the full list of projects in the system:\n"
                     + (projects.isEmpty() ? "Aucun projet trouvé.\n" : projects.join("\n") + "\n\n")
                     + "Answer the following question using this list.\n"
                       "Question: " + question;

    sendToGemma(question, prompt, 0.3); // Very deterministic
}
void SmartContentCreationStore::sendToGemma(const QString& userQuestion,const QString& fullPrompt,double temperature)
{
    // Format user message (right-aligned)
    QString userHtml = QString(
                           "<div style='text-align:right; margin:8px 0;'>"
                           "<span style='background-color:#E0F7FA; color:black; padding:10px 14px; "
                           "border-radius:14px; display:inline-block; max-width:80%; word-wrap:break-word;'>"
                           "<b>👤 You</b><br>%1"
                           "</span>"
                           "</div>"
                           ).arg(userQuestion.toHtmlEscaped());  // ✅ Use userQuestion

    ui->ChatResponse->setHtml(userHtml);
    m_dotCount = 0;
    m_dotsTimer->start(400);

    QNetworkRequest request(QUrl("http://127.0.0.1:11434/api/generate"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["model"] = "gemma:2b";
    json["prompt"] = fullPrompt;
    json["stream"] = false;
    json["options"] = QJsonObject{
        {"num_ctx", 4096},
        {"temperature", temperature}  // ← Dynamic
    };

    QNetworkReply *reply = m_networkManager->post(request, QJsonDocument(json).toJson());
    connect(reply, &QNetworkReply::finished, this, [this, reply, userHtml]() {
        m_dotsTimer->stop();
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QString response = doc.object()["response"].toString().trimmed();

            QString aiHtml = QString(
                                 "<div style='text-align:left; margin:8px 0;'>"
                                 "<span style='background-color:#F1F8E9; color:black; padding:10px 14px; "
                                 "border-radius:14px; display:inline-block; max-width:80%; word-wrap:break-word;'>"
                                 "<b>🤖 SCS.AI</b><br>%1"
                                 "</span>"
                                 "</div>"
                                 ).arg(
                                     response.toHtmlEscaped()
                                         .replace("\n", "<br>")
                                         .replace("•", "&bull;&nbsp;")
                                         .replace("✅", "&#x2705;&nbsp;")
                                         .replace("❌", "&#x274C;&nbsp;")
                                     );

            ui->ChatResponse->setHtml(userHtml + "<br>" + aiHtml);
            ui->ChatResponse->verticalScrollBar()->setValue(
                ui->ChatResponse->verticalScrollBar()->maximum()
                );
        } else {
            ui->ChatResponse->setHtml(
                "<p style='color:#D32F2F; padding:10px;'>"
                "❌ Erreur: Impossible de se connecter à Ollama.<br>"
                "Vérifiez que :<br>"
                "• Ollama est installé<br>"
                "• Le modèle 'gemma:2b' est téléchargé<br>"
                "• Le serveur tourne sur http://127.0.0.1:11434"
                "</p>"
                );
        }
        reply->deleteLater();
    });
}
void SmartContentCreationStore::handleProjectRelatedQuestion(const QString& question)
{
    // Detect what kind of data the user wants
    bool wantsIDs = question.contains(QRegularExpression(R"(\b(id|ids|identifiants?|numéros?)\b)", QRegularExpression::CaseInsensitiveOption));
    bool wantsNames = question.contains(QRegularExpression(R"(\b(nom|noms|titres?|projets?)\b)", QRegularExpression::CaseInsensitiveOption));
    bool wantsStartDates = question.contains(QRegularExpression(R"(\b(date\s*début|début|start\s*date)\b)", QRegularExpression::CaseInsensitiveOption));
    bool wantsEndDates = question.contains(QRegularExpression(R"(\b(date\s*fin|fin|end\s*date)\b)", QRegularExpression::CaseInsensitiveOption));
    bool wantsStatus = question.contains(QRegularExpression(R"(\b(statut|status|état|avancement)\b)", QRegularExpression::CaseInsensitiveOption));

    QString dataSummary = "";

    if (wantsIDs || wantsNames || wantsStartDates || wantsEndDates || wantsStatus) {
        // Fetch raw project data
        QSqlQuery qList;
        if (qList.exec("SELECT ID_PROJET, NOM, DATE_DEBUT, DATE_FIN, STATUT FROM CONTENTCREATION.PROJET ORDER BY ID_PROJET")) {
            QStringList dataList;
            while (qList.next()) {
                QString line = "";
                if (wantsIDs) {
                    line += "ID " + qList.value("ID_PROJET").toString();
                }
                if (wantsNames) {
                    if (!line.isEmpty()) line += " – ";
                    line += qList.value("NOM").toString();
                }
                if (wantsStartDates) {
                    if (!line.isEmpty()) line += " | Début: ";
                    else line = "Début: ";
                    line += qList.value("DATE_DEBUT").toDate().toString("dd/MM/yyyy");
                }
                if (wantsEndDates) {
                    line += " | Fin: " + qList.value("DATE_FIN").toDate().toString("dd/MM/yyyy");
                }
                if (wantsStatus) {
                    line += " | Statut: " + qList.value("STATUT").toString();
                }
                dataList << line;
            }

            if (!dataList.isEmpty()) {
                dataSummary += "\n📋 Données demandées :\n" + dataList.join("\n") + "\n\n";
            } else {
                dataSummary += "\n📋 Aucun projet trouvé dans la base de données.\n\n";
            }
        } else {
            dataSummary += "\n⚠️ Erreur lors de la récupération des données.\n\n";
        }
    } else {
        // Fallback: compute general statistics (only if not asking for raw data)
        QSqlQuery q;
        int total = 0, completed = 0, delayed = 0, inProgress = 0;

        if (q.exec("SELECT COUNT(*) FROM CONTENTCREATION.PROJET")) {
            if (q.next()) total = q.value(0).toInt();
        }

        if (q.exec("SELECT COUNT(*) FROM CONTENTCREATION.PROJET WHERE STATUT = 'Terminé'")) {
            if (q.next()) completed = q.value(0).toInt();
        }

        if (q.exec("SELECT COUNT(*) FROM CONTENTCREATION.PROJET WHERE STATUT LIKE 'En cours%'")) {
            if (q.next()) inProgress = q.value(0).toInt();
        }

        if (q.exec("SELECT COUNT(*) FROM CONTENTCREATION.PROJET WHERE DATE_FIN < CURRENT_DATE AND STATUT != 'Terminé'")) {
            if (q.next()) delayed = q.value(0).toInt();
        }

        double delayRisk = total > 0 ? (static_cast<double>(delayed) / total) * 100.0 : 0.0;
        double completionRate = total > 0 ? (static_cast<double>(completed) / total) * 100.0 : 0.0;

        dataSummary = QString(
                          "📊 Données actuelles des projets (Smart Content Creation Store):\n"
                          "- Nombre total de projets : %1\n"
                          "- Projets terminés : %2 (%3%)\n"
                          "- Projets en cours : %4\n"
                          "- Projets en retard : %5\n"
                          "- Risque moyen de retard : %6%\n\n"
                          ).arg(total)
                          .arg(completed).arg(QString::number(completionRate, 'f', 1))
                          .arg(inProgress)
                          .arg(delayed)
                          .arg(QString::number(delayRisk, 'f', 1));
    }

    // Final prompt
    QString systemPrompt =
        "You are SCS.AI, the official AI assistant of Smart Content Creation Store (Tunisia).\n"
        "You specialize in project management and data analysis.\n\n"
        + dataSummary +
        "✅ Answer the following question using the data above. Be precise and helpful.\n"
        "❌ Never invent data. If unsure, say: \"Je n’ai pas cette information dans le système.\"\n\n"
        "Question: " + question;

    sendToGemma(question, systemPrompt, 0.4); // Low creativity for accuracy
}


