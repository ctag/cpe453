#-------------------------------------------------
#
# Project created by QtCreator 2015-02-08T22:37:59
#
#-------------------------------------------------

QT       +=  core gui

INCLUDEPATH += $$PWD

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NodeToNode
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mywidget.cpp

HEADERS  += mainwindow.h \
    mywidget.h

FORMS    += mainwindow.ui \

