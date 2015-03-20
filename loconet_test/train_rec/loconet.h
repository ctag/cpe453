#ifndef LOCONET_H
#define LOCONET_H

#include <QString>
#include <QDebug>
#include <QChar>
#include <QBitArray>
#include <QByteArray>
#include <QVector>
#include <QTimer>
#include <QObject>
#include <QThread>

#include "locoblock.h"
#include "locobyte.h"
#include "locotrain.h"
#include "locopacket.h"
#include "locoserial.h"
#include "locosql.h"

namespace std {
class LocoNet;
}

class LocoNet : public QObject
{

    Q_OBJECT

public:
    LocoNet();
    ~LocoNet();
    //bool get_serialOpen();
    bool get_timerActive();
    QVector<LocoPacket> get_timerPackets();
    int get_timerPacketInterval(LocoPacket _packet);
    QVector<LocoTrain> get_trains();
    QVector<LocoBlock> get_blocks();
    //void set_trackUpdatePeriod(int _seconds);
    //void set_switchUpdatePeriod(int _seconds);
    //void set_trainUpdatePeriod(int _seconds);
    //void set_packetTimer(int _msec = 200);
    //void do_addTimerPacket(LocoPacket _packet, int _interval = 1);
    //void do_stopTimerPacket(LocoPacket _packet);
    //void do_stopPacketTimer();
    //void do_addStaticOP(QString _hex, QString _name, QString _desc);
    int get_staticOPsize();
    QString get_staticOPname(int);
    QString get_staticOPhex(int);
    static bool debug;

public slots:
    //QString handle_parsePacket(LocoPacket _packet, bool _enable = 1);
    void do_serialClose();
    //void set_switches();
    void do_serialOpen(QSerialPortInfo _port);
    void do_serialWrite(LocoPacket _packet);
    void do_serialWrite(QString _hex);
    void do_sqlOpen(QString hostname, int port, QString database, QString username, QString password);
    void do_sqlClose();

signals:
    void receivedPacket(LocoPacket);
    void newPacketDescription(QString);
    void trainUpdated(LocoTrain);
    void blockUpdated(LocoBlock);
    void DBopened();
    void DBclosed();
    void serialOpened();
    void serialClosed();
    void droppedPacket();

protected:

protected slots:
    //void handle_serialRead(LocoPacket _packet);
    //void handle_serialRead();
    //void handle_packetTimer();

private:
    //QVector<LocoTrain> trains;
    //QVector<LocoBlock> blocks;
    //static QVector<LocoByte> opcodes_hex;
    //static QVector<QString> opcodes_name;
    //static QVector<QString> opcodes_desc;
    //QTimer * packetTimer;
    //QVector<LocoPacket> packetTimerPackets;
    //QVector<int> packetTimerPacketState;
    //QVector<int> packetTimerPacketInterval;
    //LocoPacket incomingPacket;
    LocoSerial locoserial;
    QThread serialThread;
    LocoSQL locosql;
    QThread sqlThread;
};

#endif // LOCONET_H







