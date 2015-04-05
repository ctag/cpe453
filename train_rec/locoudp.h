#ifndef LOCOUDP_H
#define LOCOUDP_H

#include <QString>
#include <QDebug>
#include <QChar>
#include <QUdpSocket>
#include <QByteArray>
#include <QPointer>

#include "locobyte.h"
#include "locoblock.h"
#include "locotrain.h"
#include "locopacket.h"

namespace std {
class LocoUDP;
}

class LocoUDP : public QObject
{
    Q_OBJECT

public:
    LocoUDP();
    ~LocoUDP();
    static bool debug;

public slots:
    void do_run(int _port);
    void do_closeSocket();
    void do_readPendingDatagram();

signals:
    void incomingRequest(LocoPacket _packet);

protected:
    QPointer<QUdpSocket> socket;
    LocoPacket * request;
};

#endif // LOCOUDP_H







