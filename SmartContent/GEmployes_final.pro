# -------------------------------------------------
# Projet : Atelier_Connexion
# Auteur : Vous
# Date   : 2025
# -------------------------------------------------

# --- Modules Qt requis ---
QT += core gui widgets sql network charts printsupport charts

# --- Configuration du projet ---
TARGET = Atelier_Connexion
TEMPLATE = app
CONFIG += c++17

# --- Avertissements ---
DEFINES += QT_DEPRECATED_WARNINGS

# --- Chemins OpenCV (si utilisé) ---
# ⚠️ Remplace par ton propre chemin si nécessaire
#INCLUDEPATH += C:/Users/ismai/Desktop/d/ismailzac/apps/opencv/build/include
#LIBS += -LC:/Users/ismai/Desktop/d/ismailzac/apps/opencv/build/x64/vc16/lib
#LIBS += -lopencv_world4120

# --- Fichiers sources ---
SOURCES += \
    chatbot.cpp \
    connection.cpp \
    employes.cpp \
    evenement.cpp \
    main.cpp \
    projet.cpp \
    smart_cont_creat_stud.cpp

# --- Fichiers en-tête ---
HEADERS += \
    chatbot.h \
    connection.h \
    employes.h \
    evenement.h \
    projet.h \
    smart_cont_creat_stud.h

# --- Fichiers UI ---
FORMS += \
    smart_cont_creat_stud.ui

# --- Ressources ---
RESOURCES += \
    images.qrc

# --- Fichiers de données à inclure dans le package ---
DISTFILES += \
    data/connaissances.txt \
    data/memoire.txt

# --- Copier le dossier data/ dans le répertoire de sortie (Windows) ---
win32 {
    copydata.commands = $$quote(robocopy \"$$PWD\\data\" \"$$OUT_PWD\\data\" /E /NFL /NDL /NJH /NJS)
    first.depends = $(first) copydata
    export(first.depends)
    QMAKE_EXTRA_TARGETS += first copydata
}

# --- Installation (optionnel) ---
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
