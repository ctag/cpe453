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

#include "locoblock.h"
#include "locobyte.h"
#include "locotrain.h"
#include "locopacket.h"
#include "locoserial.h"
#include "locosql.h"

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
    void do_enableArgs();
    void do_genPacket();
    void do_refreshSerialList();
    void do_openSerial();
    void do_closeSerial();
    void do_dumpQByteArray(QByteArray _data);
    void do_sendSerial();
    void do_connectDB();
    void do_disconnectDB();
    void do_displayPacket(LocoPacket);
    void do_printDescriptions(QString);
    void do_loadOPComboBox();
    void do_OPfromComboBox();

    void handle_serialOpened();
    void handle_serialClosed();
    void handle_DBopened();
    void handle_DBclosed();

private:
    Ui::MainWindow *ui;
    LocoPacket outgoingPacket;
    LocoPacket incomingPacket;
    //LocoNet loconet;
    QSerialPortInfo usbPorts;
    QDateTime date;
    LocoSerial locoserial;
    QThread serialThread;
    LocoSQL locosql;
    QThread sqlThread;
};

#endif // MAINWINDOW_H
