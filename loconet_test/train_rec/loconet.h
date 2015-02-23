#ifndef LOCONET_H
#define LOCONET_H

#include <QString>
#include <QDebug>
#include <QChar>
#include <QBitArray>
#include <QByteArray>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QVector>
#include <QObject>

#include "locobyte.h"
#include "locotrain.h"
#include "locopacket.h"

namespace std {
class LocoNet;
}

class LocoNet : public QObject
{

    Q_OBJECT

public:
    LocoNet();
    ~LocoNet();
    bool get_serialOpen();
    QVector<LocoTrain> get_trains();
    bool do_serialOpen(QSerialPortInfo _port);
    void do_serialClose();
    void do_serialSend(LocoPacket _packet);
    void do_findTrains();

//public slots:

protected:
    void do_serialRead();
    void do_serialWrite();

signals:
    void newPacket(LocoPacket _packet);

private:
    QVector<LocoTrain> trains;
    QSerialPort * usbBuffer;
    LocoPacket incomingPacket;
    static bool debug;

};

#endif // LOCONET_H







