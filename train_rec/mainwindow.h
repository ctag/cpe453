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
#include "locoudp.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString timeStamp();
    static bool debug;

public slots:
    void do_enableArgs();
    void do_genPacket();
    void do_refreshSerialList();
    void do_openSerial();
    void do_sendSerial();
    void do_connectDB();
    void do_packetReceived(LocoPacket);
    void do_bytesWritten(QByteArray);
    void do_printDescriptions(QString);
    void do_loadOPComboBox();
    void do_OPfromComboBox();

    void handle_serialOpened();
    void handle_serialClosed();
    void handle_DBopened();
    void handle_DBclosed();

signals:
    void locoserial_open(QSerialPortInfo _device);
    void locoserial_write(LocoPacket outgoingPacket);
    void locosql_open(QString hostname, int port, QString database, QString username, QString password);

private:
    Ui::MainWindow *ui;
    LocoPacket outgoingPacket;
    LocoPacket incomingPacket;
    QSerialPortInfo usbPorts;
    LocoSerial * locoserial;
    QThread threadSerial;
    LocoSQL * locosql;
    QThread threadSQL;
    LocoUDP * locoudp;
    QThread threadUDP;
};

#endif // MAINWINDOW_H
