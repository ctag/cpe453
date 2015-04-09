#-------------------------------------------------
#
# Project created by QtCreator 2015-02-08T22:37:59
#
#-------------------------------------------------

QT       +=  core gui widgets network sql

INCLUDEPATH += $$PWD

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NodeToNode
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    vertex.cpp \
    track.cpp \
    sql.cpp \
    edge.cpp \
    text.cpp

HEADERS  += mainwindow.h \
    vertex.h \
    track.h \
    sql.h \
    edge.h \
    text.h

FORMS    += mainwindow.ui \

