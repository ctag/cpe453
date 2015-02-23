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
    void do_findTrains();
    void do_serialWrite(LocoPacket _packet);
    void do_serialWrite(QString _hex);

public slots:
    QString do_parsePacket(LocoPacket _packet);
    void do_serialClose();

signals:
    void newPacket(LocoPacket _packet);

protected:
    QString handle_E7(LocoPacket _packet);

protected slots:
    void do_serialRead();

private:
    QVector<LocoTrain> trains;
    QSerialPort * usbBuffer;
    LocoPacket incomingPacket;
    static bool debug;

};

#endif // LOCONET_H







