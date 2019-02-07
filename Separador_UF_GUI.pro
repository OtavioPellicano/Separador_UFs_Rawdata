#-------------------------------------------------
#
# Project created by QtCreator 2019-02-06T10:16:38
#
#-------------------------------------------------

QT       += core gui
QT += concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Separador_UF_GUI
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        MainWindow.cpp \
    core/Log.cpp \
    core/Rawdata.cpp \
    core/Rawdata_Organizer.cpp \
    core/Scm_4_5.cpp \
    core/Scm_6.cpp \
    core/Scm_7.cpp \
    core/Scm_8.cpp \
    core/Scm_9.cpp \
    core/Smp_10_11.cpp

HEADERS += \
        MainWindow.h \
    core/Log.h \
    core/Rawdata.h \
    core/Rawdata_Organizer.h \
    core/Scm_4_5.h \
    core/Scm_6.h \
    core/Scm_7.h \
    core/Scm_8.h \
    core/Scm_9.h \
    core/Smp_10_11.h \
    core/Thread_Guard.h

FORMS += \
        MainWindow.ui

RESOURCES += \
    img.qrc
