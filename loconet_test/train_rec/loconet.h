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
#include <QTimer>
#include <QObject>

#include "locorecv.h"
#include "locoblock.h"
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
    bool get_timerActive();
    QVector<LocoPacket> get_timerPackets();
    int get_timerPacketInterval(LocoPacket _packet);
    QVector<LocoTrain> get_trains();
    QVector<LocoBlock> get_blocks();
    void set_trackUpdatePeriod(int _seconds);
    void set_switchUpdatePeriod(int _seconds);
    void set_trainUpdatePeriod(int _seconds);
    bool do_serialOpen(QSerialPortInfo _port);
    void do_serialWrite(LocoPacket _packet);
    void do_serialWrite(QString _hex);
    void set_packetTimer(int _msec = 200);
    void do_addTimerPacket(LocoPacket _packet, int _interval = 1);
    void do_stopTimerPacket(LocoPacket _packet);
    void do_stopPacketTimer();
    void do_addStaticOP(QString _hex, QString _name, QString _desc);
    int get_staticOPsize();
    QString get_staticOPname(int);
    QString get_staticOPhex(int);

public slots:
    QString handle_parsePacket(LocoPacket _packet, bool _enable = 1);
    void do_serialClose();
    //void set_switches();

signals:
    void newPacket(LocoPacket);
    void newPacketDescription(QString);
    void trainUpdated(LocoTrain);
    void blockUpdated(LocoBlock);

protected:
    QString parse_E7(LocoPacket _packet, bool _enable);
    QString parse_EF(LocoPacket _packet, bool _enable);
    QString parse_E5(LocoPacket _packet, bool _enable);
    QString parse_ED(LocoPacket _packet, bool _enable);
    QString parse_B2(LocoPacket _packet, bool _enable);
    QString parse_85(LocoPacket _packet, bool _enable);
    QString parse_83(LocoPacket _packet, bool _enable);
    QString parse_82(LocoPacket _packet, bool _enable);
    QString parse_81(LocoPacket _packet, bool _enable);
    QString parse_BF(LocoPacket _packet, bool _enable);
    QString parse_BD(LocoPacket _packet, bool _enable);
    QString parse_BC(LocoPacket _packet, bool _enable);
    QString parse_BB(LocoPacket _packet, bool _enable);
    QString parse_BA(LocoPacket _packet, bool _enable);
    QString parse_B9(LocoPacket _packet, bool _enable);
    QString parse_B8(LocoPacket _packet, bool _enable);
    QString parse_B6(LocoPacket _packet, bool _enable);
    QString parse_B5(LocoPacket _packet, bool _enable);
    QString parse_B4(LocoPacket _packet, bool _enable);
    QString parse_B1(LocoPacket _packet, bool _enable);
    QString parse_B0(LocoPacket _packet, bool _enable);
    QString parse_A2(LocoPacket _packet, bool _enable);
    QString parse_A1(LocoPacket _packet, bool _enable);
    QString parse_A0(LocoPacket _packet, bool _enable);

protected slots:
    void handle_packetTimer();
    void handle_serialRead(LocoPacket _packet);
    void handle_serialRead();

private:
    QVector<LocoTrain> trains;
    QVector<LocoBlock> blocks;
    QSerialPort * usbBuffer;
    static bool debug;
    static QVector<LocoByte> opcodes_hex;
    static QVector<QString> opcodes_name;
    static QVector<QString> opcodes_desc;
    QTimer * packetTimer;
    QVector<LocoPacket> packetTimerPackets;
    QVector<int> packetTimerPacketState;
    QVector<int> packetTimerPacketInterval;
    //QThread usbThread;
    //LocoRecv * locorecv;
    LocoPacket incomingPacket;
};

#endif // LOCONET_H







