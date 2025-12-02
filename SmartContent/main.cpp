#include "smart_cont_creat_stud.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Connexion à la base
    Connection* c = Connection::instance();
    bool test = c->createConnect();

    if (!test) {
        QMessageBox::critical(nullptr, QObject::tr("Base de données"),
                              QObject::tr("Échec de la connexion.\nVérifiez la configuration."));
        return -1;
    } else {
        QMessageBox::information(nullptr, QObject::tr("Base de données"),
                                 QObject::tr("Connexion réussie.\nCliquez sur OK pour continuer."));
    }

    // Création et affichage de la fenêtre principale intégrée
    SmartContentCreationStore w; // contient maintenant les pages Employés + Événements
    w.show();

    return a.exec();
}
