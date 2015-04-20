#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // These are important if you wish to use SQL as a thread!
    qRegisterMetaType< QVector<int> >("QVector<int>");
    qRegisterMetaType< QVector<QString> >("QVector<QString>");

    // for passing vertices around
    qRegisterMetaType< QList<vertex*> >("QList<vertex*>");

    return a.exec();
}
