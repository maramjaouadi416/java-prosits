#include "evenement.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QTableWidgetItem>
#include <QObject>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QChart>
#include <QPainter>
#include <QSqlDatabase>
#include <QPrinter>
#include <QTextDocument>
#include <QTextTable>
#include <QTextCursor>
#include <QDateTime>
#include <QPageSize>       // ✅ Ajouté
#include <QPageLayout>     // ✅ Ajouté (pour les marges)

Evenement::Evenement() {}

Evenement::Evenement(const QString &nom, const QString &type,
                     const QString &debut, const QString &fin,
                     const QString &lieu)
    : nom(nom), type(type), debut(debut), fin(fin), lieu(lieu)
{
}

bool Evenement::ajouter()
{
    if (debut.length() != 10 || fin.length() != 10) {
        qDebug() << "❌ Format de date invalide (attendu: yyyy-MM-dd)";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO EVENEMENT (NOM, TYPE, DEBUT, FIN, LIEU) "
                  "VALUES (:nom, :type, :debut, :fin, :lieu)");

    query.bindValue(":nom", nom);
    query.bindValue(":type", type);
    query.bindValue(":debut", debut);
    query.bindValue(":fin", fin);
    query.bindValue(":lieu", lieu);

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL (ajout):" << query.lastError().text();
        return false;
    }

    qDebug() << "✅ Événement ajouté avec succès !";
    return true;
}

bool Evenement::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM EVENEMENT WHERE ID_EVENT = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "❌ Erreur suppression:" << query.lastError().text();
        return false;
    }

    qDebug() << "🗑️ Événement supprimé avec succès (ID:" << id << ")";
    return true;
}

bool Evenement::modifier(int id, const QString &nom, const QString &type,
                         const QString &debut, const QString &fin,
                         const QString &lieu)
{
    QSqlQuery query;
    query.prepare("UPDATE EVENEMENT "
                  "SET NOM = :nom, TYPE = :type, DEBUT = :debut, FIN = :fin, LIEU = :lieu "
                  "WHERE ID_EVENT = :id");

    query.bindValue(":nom", nom);
    query.bindValue(":type", type);
    query.bindValue(":debut", debut);
    query.bindValue(":fin", fin);
    query.bindValue(":lieu", lieu);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "❌ Erreur modification:" << query.lastError().text();
        return false;
    }

    qDebug() << "✏️ Événement modifié avec succès (ID:" << id << ")";
    return true;
}

bool Evenement::chargerDonnees(QTableWidget *table)
{
    if (!table) return false;

    table->setRowCount(0);
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels({
        QObject::tr("ID"), QObject::tr("Nom"), QObject::tr("Type"),
        QObject::tr("Début"), QObject::tr("Fin"), QObject::tr("Lieu")
    });

    QSqlQuery query("SELECT ID_EVENT, NOM, TYPE, DEBUT, FIN, LIEU "
                    "FROM EVENEMENT ORDER BY ID_EVENT");

    while (query.next()) {
        int row = table->rowCount();
        table->insertRow(row);

        table->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        table->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        table->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
        table->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
        table->setItem(row, 4, new QTableWidgetItem(query.value(4).toString()));
        table->setItem(row, 5, new QTableWidgetItem(query.value(5).toString()));
    }
    query.finish();
    table->resizeColumnsToContents();
    return true;
}

bool Evenement::chargerDonneesTrieParType(QTableWidget *table, const QString &typeCible)
{
    if (!table) return false;

    table->setRowCount(0);
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels({
        QObject::tr("ID"), QObject::tr("Nom"), QObject::tr("Type"),
        QObject::tr("Début"), QObject::tr("Fin"), QObject::tr("Lieu")
    });

    QSqlQuery query;
    query.prepare("SELECT ID_EVENT, NOM, TYPE, DEBUT, FIN, LIEU FROM EVENEMENT");
    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL:" << query.lastError().text();
        return false;
    }

    QList<QList<QVariant>> cibleRows;
    QMap<QString, QList<QList<QVariant>>> autresGroupes;

    // 📥 Charger les lignes SANS TRI INTERNE
    while (query.next()) {
        QList<QVariant> row;
        for (int i = 0; i < 6; ++i) {
            row.append(query.value(i));
        }

        QString type = query.value(2).toString();
        if (type.compare(typeCible, Qt::CaseInsensitive) == 0) {
            cibleRows.append(row);
        } else {
            autresGroupes[type].append(row);
        }
    }

    // ➕ Fonction pour ajouter des lignes au tableau
    auto ajouterLignes = [&](const QList<QList<QVariant>> &rows) {
        for (const auto &row : rows) {
            int r = table->rowCount();
            table->insertRow(r);
            for (int col = 0; col < 6; ++col) {
                table->setItem(r, col, new QTableWidgetItem(row[col].toString()));
            }
        }
    };

    // ✅ 1. Ajouter le groupe cible (dans l'ordre d'origine)
    ajouterLignes(cibleRows);

    // ✅ 2. Extraire les autres types et les trier ALPHABÉTIQUEMENT (par nom du type)
    QList<QString> typesTries = autresGroupes.keys();
    std::sort(typesTries.begin(), typesTries.end(), [](const QString &a, const QString &b) {
        return a.toLower() < b.toLower(); // tri insensible à la casse
    });

    // ✅ 3. Ajouter les autres groupes dans l'ordre alphabétique des types
    //     → MAIS sans trier les événements à l'intérieur
    for (const QString &type : typesTries) {
        ajouterLignes(autresGroupes[type]);
    }

    table->resizeColumnsToContents();
    return true;
}

bool Evenement::rechercherParNomSQL(QTableWidget *table, const QString &nomRecherche)
{
    if (!table) return false;

    table->setRowCount(0);

    if (nomRecherche.isEmpty()) {
        return true;
    }

    QSqlQuery query;
    query.prepare("SELECT ID_EVENT, NOM, TYPE, DEBUT, FIN, LIEU "
                  "FROM EVENEMENT "
                  "WHERE NOM LIKE :nomRecherche "
                  "ORDER BY ID_EVENT");

    query.bindValue(":nomRecherche", "%" + nomRecherche + "%");

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL recherche événement:" << query.lastError().text();
        return false;
    }

    while (query.next()) {
        int row = table->rowCount();
        table->insertRow(row);

        table->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        table->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        table->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
        table->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
        table->setItem(row, 4, new QTableWidgetItem(query.value(4).toString()));
        table->setItem(row, 5, new QTableWidgetItem(query.value(5).toString()));
    }
    query.finish();
    table->resizeColumnsToContents();
    return true;
}

void Evenement::StatistiquesParType(QChartView *chartView)
{
    if (!chartView)
        return;

    if (!QSqlDatabase::database().isOpen()) {
        qDebug() << "⚠️ Base non ouverte dans StatistiquesParType";
        return;
    }

    QSqlQuery query("SELECT TYPE, COUNT(*) FROM EVENEMENT GROUP BY TYPE");
    QMap<QString, int> statsParType;

    while (query.next()) {
        statsParType[query.value(0).toString()] = query.value(1).toInt();
    }

    QBarSet *set = new QBarSet(QObject::tr("Nombre d'événements"));
    QStringList labels = statsParType.keys();
    for (const QString &type : labels) {
        *set << statsParType.value(type, 0);
    }

    QBarSeries *series = new QBarSeries();
    series->append(set);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(QObject::tr("Statistiques des événements par type"));
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(false);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(labels);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText(QObject::tr("Nombre d'événements"));
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
}

bool Evenement::exporterTousEvenementsEnPDF(const QString &filePath)
{
    QTextDocument doc;
    QTextCursor cursor(&doc);

    QTextBlockFormat titleBlockFormat;
    titleBlockFormat.setAlignment(Qt::AlignCenter);
    QTextCharFormat titleCharFormat;
    titleCharFormat.setFontPointSize(16);
    titleCharFormat.setFontWeight(QFont::Bold);
    cursor.insertBlock(titleBlockFormat);
    cursor.insertText(QObject::tr("Tous les Événements – SmartContentCreationStore"), titleCharFormat);
    cursor.insertBlock();
    cursor.insertBlock();

    QSqlQuery query;
    query.prepare("SELECT ID_EVENT, NOM, TYPE, DEBUT, FIN, LIEU "
                  "FROM EVENEMENT "
                  "ORDER BY ID_EVENT");

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL export PDF:" << query.lastError().text();
        return false;
    }

    QTextTableFormat tableFormat;
    tableFormat.setBorder(0.5);
    tableFormat.setCellPadding(6);
    QList<QTextLength> constraints;
    for (int i = 0; i < 6; ++i)
        constraints << QTextLength(QTextLength::PercentageLength, 100.0 / 6.0);
    tableFormat.setColumnWidthConstraints(constraints);

    QTextTable *table = cursor.insertTable(1, 6, tableFormat);

    QStringList headers = { "ID", "Nom", "Type", "Début", "Fin", "Lieu" };
    for (int i = 0; i < 6; ++i)
        table->cellAt(0, i).firstCursorPosition().insertText(headers[i]);

    while (query.next()) {
        int row = table->rows();
        table->insertRows(row, 1);
        for (int col = 0; col < 6; ++col) {
            QString text = query.value(col).toString();
            if (col == 3 || col == 4) {
                QDateTime dt = query.value(col).toDateTime();
                if (dt.isValid()) text = dt.toString("dd/MM/yyyy");
            }
            table->cellAt(row, col).firstCursorPosition().insertText(text);
        }
    }

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    printer.setPageSize(QPageSize(QPageSize::A4)); // ✅ Correct
    printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter); // ✅ Correct
    doc.print(&printer);
    return true;
}



// ===================================================================================
// ====== NOUVELLES FONCTIONS POUR DÉPLACER LA LOGIQUE SQL DE SmartContentCreationStore
// ===================================================================================

// 🔹 Chargement des valeurs uniques
QList<int> Evenement::recupererTousIDs()
{
    QList<int> ids;
    QSqlQuery query("SELECT ID_EVENT FROM EVENEMENT ORDER BY ID_EVENT");
    while (query.next()) {
        ids.append(query.value(0).toInt());
    }
    query.finish(); // ✅ Libère la connexion ODBC
    return ids;
}

QStringList Evenement::recupererTousNomsUniques()
{
    QSet<QString> nomsUniques;
    QSqlQuery query("SELECT NOM FROM EVENEMENT");
    while (query.next()) {
        nomsUniques.insert(query.value(0).toString());
    }
    query.finish(); // ✅
    return nomsUniques.values();
}

QStringList Evenement::recupererTousTypesUniques()
{
    QSet<QString> typesUniques;
    QSqlQuery query("SELECT TYPE FROM EVENEMENT");
    while (query.next()) {
        typesUniques.insert(query.value(0).toString());
    }
    query.finish(); // ✅
    return typesUniques.values();
}

QStringList Evenement::recupererTousLieuxUniques()
{
    QSet<QString> lieuxUniques;
    QSqlQuery query("SELECT LIEU FROM EVENEMENT");
    while (query.next()) {
        lieuxUniques.insert(query.value(0).toString());
    }
    query.finish(); // ✅
    return lieuxUniques.values();
}

// 🔹 Réponses par critère
QString Evenement::repondreParID(int id)
{
    QSqlQuery q;
    q.prepare("SELECT NOM, TYPE, DEBUT, FIN, LIEU FROM EVENEMENT WHERE ID_EVENT = ?");
    q.addBindValue(id);
    if (q.exec() && q.next()) {
        QString reponse = QString("Événement #%1 :\n"
                                  "- Nom : %2\n"
                                  "- Type : %3\n"
                                  "- Début : %4\n"
                                  "- Fin : %5\n"
                                  "- Lieu : %6")
                              .arg(id)
                              .arg(q.value(0).toString())
                              .arg(q.value(1).toString())
                              .arg(q.value(2).toString())
                              .arg(q.value(3).toString())
                              .arg(q.value(4).toString());
        q.finish(); // ✅
        return reponse;
    }
    q.finish(); // ✅ (même en cas d'échec)
    return QString("Aucun événement trouvé avec l'ID %1.").arg(id);
}

QString Evenement::repondreParNom(const QString &nom)
{
    QSqlQuery q;
    q.prepare("SELECT ID_EVENT, TYPE, DEBUT, FIN, LIEU FROM EVENEMENT WHERE NOM = ?");
    q.addBindValue(nom);
    QString reponse = "Événement(s) trouvés avec le nom \"" + nom + "\" :\n";
    bool found = false;
    if (q.exec()) {
        while (q.next()) {
            reponse += QString("• ID %1 | Type : %2 | %3 → %4 | %5\n")
                           .arg(q.value(0).toString())
                           .arg(q.value(1).toString())
                           .arg(q.value(2).toString())
                           .arg(q.value(3).toString())
                           .arg(q.value(4).toString());
            found = true;
        }
    }
    q.finish(); // ✅
    return found ? reponse.trimmed() : "Aucun événement trouvé avec ce nom.";
}

QString Evenement::repondreParType(const QString &type)
{
    QSqlQuery q;
    q.prepare("SELECT ID_EVENT, NOM, TYPE, DEBUT, FIN, LIEU FROM EVENEMENT WHERE TYPE = ?");
    q.addBindValue(type);
    QString reponse = "Événements de type \"" + type + "\" :\n";
    bool found = false;
    if (q.exec()) {
        while (q.next()) {
            reponse += QString("• %1 (%2) du %3 au %4 à %5 [ID %6]\n")
                           .arg(q.value(1).toString()) // NOM
                           .arg(q.value(2).toString()) // TYPE
                           .arg(q.value(3).toString()) // DEBUT
                           .arg(q.value(4).toString()) // FIN
                           .arg(q.value(5).toString()) // LIEU
                           .arg(q.value(0).toString()); // ID
            found = true;
        }
    }
    q.finish(); // ✅
    return found ? reponse.trimmed() : "Aucun événement de ce type.";
}

QString Evenement::repondreParLieu(const QString &lieu)
{
    QSqlQuery q;
    q.prepare("SELECT ID_EVENT, NOM, TYPE, DEBUT, FIN FROM EVENEMENT WHERE LIEU = ?");
    q.addBindValue(lieu);
    QString reponse = "Événements à \"" + lieu + "\" :\n";
    bool found = false;
    if (q.exec()) {
        while (q.next()) {
            reponse += QString("• %1 (%2) du %3 au %4 [ID %5]\n")
                           .arg(q.value(1).toString())
                           .arg(q.value(2).toString())
                           .arg(q.value(3).toString())
                           .arg(q.value(4).toString())
                           .arg(q.value(0).toString());
            found = true;
        }
    }
    q.finish(); // ✅
    return found ? reponse.trimmed() : "Aucun événement à ce lieu.";
}

QString Evenement::repondreParDate(const QDate &date)
{
    QString dateStr = date.toString("yyyy-MM-dd");
    QSqlQuery q;
    q.prepare("SELECT ID_EVENT, NOM, TYPE, LIEU FROM EVENEMENT WHERE DEBUT = ?");
    q.addBindValue(dateStr);
    QString reponse = "Événements prévus le " + dateStr + " :\n";
    bool found = false;
    if (q.exec()) {
        while (q.next()) {
            reponse += QString("• %1 (%2) à %3 [ID %4]\n")
                           .arg(q.value(1).toString())
                           .arg(q.value(2).toString())
                           .arg(q.value(3).toString())
                           .arg(q.value(0).toString());
            found = true;
        }
    }
    q.finish(); // ✅
    return found ? reponse.trimmed() : "Aucun événement trouvé à cette date.";
}

int Evenement::compterTousEvenements()
{
    QSqlQuery q("SELECT COUNT(*) FROM EVENEMENT");
    if (q.exec() && q.next()) {
        int result = q.value(0).toInt();
        q.finish(); // ✅
        return result;
    }
    q.finish(); // ✅
    return 0;
}

QString Evenement::listerTousEvenements()
{
    QSqlQuery q;
    if (!q.exec("SELECT ID_EVENT, NOM, TYPE, DEBUT, LIEU FROM EVENEMENT ORDER BY ID_EVENT")) {
        return "Erreur lors de la récupération des événements : " + q.lastError().text();
    }

    QString reponse = "Liste de tous les événements :\n";
    bool found = false;
    while (q.next()) {
        reponse += QString("• [%1] %2 (%3) le %4 à %5\n")
                       .arg(q.value(0).toString())
                       .arg(q.value(1). toString())
                       .arg(q.value(2).toString())
                       .arg(q.value(3).toString())
                       .arg(q.value(4).toString());
        found = true;
    }
    q.finish();
    return found ? reponse.trimmed() : "Aucun événement enregistré.";
}
QString Evenement::listerEvenementsAFutur(const QDate &aujourdhui)
{
    QSqlQuery q;
    q.prepare("SELECT ID_EVENT, NOM, TYPE, DEBUT, LIEU FROM EVENEMENT WHERE DEBUT >= ?");
    q.addBindValue(aujourdhui.toString("yyyy-MM-dd"));
    QString reponse = "Événements à venir :\n";
    bool found = false;
    if (q.exec()) {
        while (q.next()) {
            reponse += QString("• %1 (%2) le %3 à %4 [ID %5]\n")
                           .arg(q.value(1).toString())
                           .arg(q.value(2).toString())
                           .arg(q.value(3).toString())
                           .arg(q.value(4).toString())
                           .arg(q.value(0).toString());
            found = true;
        }
    }
    q.finish(); // ✅
    return found ? reponse.trimmed() : "Aucun événement à venir.";
}

QString Evenement::listerEvenementsAujourdhui(const QDate &aujourdhui)
{
    QString dateStr = aujourdhui.toString("yyyy-MM-dd");
    QSqlQuery q;
    q.prepare("SELECT ID_EVENT, NOM, TYPE, LIEU FROM EVENEMENT WHERE DEBUT = ?");
    q.addBindValue(dateStr);
    QString reponse = "Événements aujourd'hui :\n";
    bool found = false;
    if (q.exec()) {
        while (q.next()) {
            reponse += QString("• %1 (%2) à %3 [ID %4]\n")
                           .arg(q.value(1).toString())
                           .arg(q.value(2).toString())
                           .arg(q.value(3).toString())
                           .arg(q.value(0).toString());
            found = true;
        }
    }
    q.finish(); // ✅
    return found ? reponse.trimmed() : "Aucun événement aujourd'hui.";
}

// 🔹 Notifications
QList<QPair<QString, QDate>> Evenement::recupererEvenementsImminents(int nbJours)
{
    QList<QPair<QString, QDate>> result;
    QDate aujourdhui = QDate::currentDate();
    QDate limite = aujourdhui.addDays(nbJours);

    QSqlQuery q;
    q.prepare("SELECT NOM, DEBUT FROM EVENEMENT "
              "WHERE DEBUT >= ? AND DEBUT <= ? "
              "ORDER BY DEBUT ASC");
    q.addBindValue(aujourdhui.toString("yyyy-MM-dd"));
    q.addBindValue(limite.toString("yyyy-MM-dd"));

    if (q.exec()) {
        while (q.next()) {
            QString nom = q.value(0).toString();
            QDate debut = QDate::fromString(q.value(1).toString(), "yyyy-MM-dd");
            result.append(qMakePair(nom, debut));
        }
    }
    q.finish(); // ✅
    return result;
}
