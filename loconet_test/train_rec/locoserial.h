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

namespace std {
class LocoSerial;
}

class LocoSerial : public QObject
{
    Q_OBJECT

public:
    LocoSerial();
    LocoSerial(QSerialPortInfo _port);
    ~LocoSerial();
    static bool debug;

signals:
    void receivedPacket(LocoPacket);
    void droppedPacket();

public slots:
    void writePacket(LocoPacket _packet);
    void writeBytes(QByteArray _bytes);
    void close();
    bool open(QSerialPortInfo _port);
    void do_read();

protected:
    void readTimerStop();
    void readTimerStart(int _msec);


protected slots:


private:
    QSerialPort usbBuffer;
    QTimer readTimer;
    LocoPacket incomingPacket;

};

#endif // LOCOSERIAL_H







