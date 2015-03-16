#ifndef LOCORECV_H
#define LOCORECV_H

#include <QThread>
#include <QDebug>
#include <QSerialPort>

#include "locopacket.h"
#include "locorecv.h"

namespace std {
class LocoRecv;
}

class LocoRecv : public QObject
{
    Q_OBJECT

public:
    LocoRecv();
    LocoRecv(QSerialPort * buffer);
    ~LocoRecv();

public slots:
    void handleRecv ();

signals:
    void receivedPacket(LocoPacket);

private:
    LocoPacket incomingPacket;
    QSerialPort * usbBuffer;
    static bool debug;

};

#endif // LOCORECV_H







