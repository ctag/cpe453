#-------------------------------------------------
#
# Project created by QtCreator 2015-01-31T18:41:09
#
#-------------------------------------------------

QT       += core gui sql network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = loconet_checksum
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
	locopacket.cpp \
    locobyte.cpp

HEADERS  += mainwindow.h \
	locopacket.h \
    locobyte.h

FORMS    += mainwindow.ui
