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

protected slots:
    void enableArgs();
    void genPacket();

private:
    Ui::MainWindow *ui;
    QSerialPort * loconet;
    LocoPacket * locopacket;
    QSerialPort usbBuffer;
    QSerialPortInfo usbPorts;
};

#endif // MAINWINDOW_H
