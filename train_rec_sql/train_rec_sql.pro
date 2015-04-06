#-------------------------------------------------
#
# Project created by QtCreator 2015-01-31T18:41:09
#
#-------------------------------------------------

QT       += core gui sql network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = train_rec_sql
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    sql.cpp

HEADERS  += mainwindow.h \
    sql.h

FORMS    += mainwindow.ui
