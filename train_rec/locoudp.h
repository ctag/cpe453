#ifndef LOCOUDP_H
#define LOCOUDP_H

#include <QString>
#include <QDebug>
#include <QChar>
#include <QUdpSocket>
#include <QByteArray>
#include <QPointer>
#include <QTime>

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
    QString timeStamp();

public slots:
    void do_run();
    void do_socketOpen(int _port);
    void do_socketClose();
    void do_readPendingDatagram();

signals:
    void incomingRequest(LocoPacket _packet);

protected:
    QPointer<QUdpSocket> socket;
    LocoPacket * request;
    bool * debug;
};

#endif // LOCOUDP_H







