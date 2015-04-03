#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QByteArray>
#include <QVariant>
#include <QComboBox>
#include <QIODevice>
#include <QTimer>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QThread>

#include "sql.h"
//#include "udp.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static bool debug;

public slots:
    void do_connectDB();
    void do_disconnectDB();

    void handle_DBopened();
    void handle_DBclosed();
    void handle_setTrain();

signals:

private:
    Ui::MainWindow *ui;
    QDateTime date;
    SQL * locosql;
    QThread threadSQL;
    //LocoUDP locoudp;
    //QThread threadUDP;
};

#endif // MAINWINDOW_H
