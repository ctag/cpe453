#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    qRegisterMetaType< QVector<int> >("QVector<int>");
    qRegisterMetaType< QVector<QString> >("QVector<QString>");

    return a.exec();
}
