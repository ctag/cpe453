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
#include "locopacket.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void do_initStaticOP(LocoPacket);
    void do_loadOPComboBox();

protected slots:
    void do_enableArgs();
    void do_genPacket();
    void do_OPfromComboBox();
    void do_serialRefreshList();
    void do_serialConnect();
    void do_serialDisconnect();
    void readSerial();
    void sendSerial();

private:
    Ui::MainWindow *ui;
    LocoPacket outgoingPacket;
    LocoPacket incomingPacket;
    QSerialPort * usbBuffer;
    QSerialPortInfo usbPorts;
};

#endif // MAINWINDOW_H