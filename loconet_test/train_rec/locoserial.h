#ifndef LOCOSERIAL_H
#define LOCOSERIAL_H

/**
 * Loco Serial Worker Class
 *
 * For threaded usb connections to Digitrax's Loconet
 */

#include <QString>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QTimer>

#include "locopacket.h"
#include "locotrain.h"
#include "locoblock.h"

namespace std {
class LocoSerial;
}

class LocoSerial : public QObject
{
    Q_OBJECT

public:
    LocoSerial();
    ~LocoSerial();
    static bool debug;

signals:
    void receivedPacket(LocoPacket);
    void droppedPacket();
    void trainUpdated(LocoTrain);
    void blockUpdated(LocoBlock);
    void serialOpened();
    void serialClosed();

public slots:
    void do_write(LocoPacket _packet);
    void do_write(QByteArray _bytes);
    void do_querySlot(LocoByte _slot);
    void do_close();
    bool do_open(QSerialPortInfo _port);
    void do_read();
    QString parse(LocoPacket _packet);
    void run();

protected:
    void readTimerStop();
    void readTimerStart(int _msec);
    QString parse_E7(LocoPacket _packet);
    QString parse_EF(LocoPacket _packet);
    QString parse_E5(LocoPacket _packet);
    QString parse_ED(LocoPacket _packet);
    QString parse_B2(LocoPacket _packet);
    QString parse_85(LocoPacket _packet);
    QString parse_83(LocoPacket _packet);
    QString parse_82(LocoPacket _packet);
    QString parse_81(LocoPacket _packet);
    QString parse_BF(LocoPacket _packet);
    QString parse_BD(LocoPacket _packet);
    QString parse_BC(LocoPacket _packet);
    QString parse_BB(LocoPacket _packet);
    QString parse_BA(LocoPacket _packet);
    QString parse_B9(LocoPacket _packet);
    QString parse_B8(LocoPacket _packet);
    QString parse_B6(LocoPacket _packet);
    QString parse_B5(LocoPacket _packet);
    QString parse_B4(LocoPacket _packet);
    QString parse_B1(LocoPacket _packet);
    QString parse_B0(LocoPacket _packet);
    QString parse_A2(LocoPacket _packet);
    QString parse_A1(LocoPacket _packet);
    QString parse_A0(LocoPacket _packet);

protected slots:

private:
    QSerialPort * usbBuffer;
    QTimer * readTimer;
    LocoPacket * incomingPacket;
    LocoPacket * outgoingPacket;

};

#endif // LOCOSERIAL_H







