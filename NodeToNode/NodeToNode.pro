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

# ECE computers don't have C++11
# :(
#CONFIG += c++11

SOURCES += main.cpp\
    mainwindow.cpp \
    vertex.cpp \
    track.cpp \
    sql.cpp \
    text.cpp \
    edge.cpp

HEADERS  += mainwindow.h \
    vertex.h \
    track.h \
    sql.h \
    text.h \
    edge.h

FORMS    += mainwindow.ui \

