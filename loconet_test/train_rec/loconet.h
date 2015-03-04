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
    QVector<LocoTrain> get_trains();
    QVector<LocoBlock> get_blocks();
    void set_trackUpdatePeriod(int _seconds);
    void set_switchUpdatePeriod(int _seconds);
    void set_trainUpdatePeriod(int _seconds);
    bool do_serialOpen(QSerialPortInfo _port);
    void do_findTrains();
    void do_serialWrite(LocoPacket _packet);
    void do_serialWrite(QString _hex);

public slots:
    QString do_parsePacket(LocoPacket _packet);
    void do_serialClose();
    void update_track();
    void update_trains();
    //void set_switches();

signals:
    void newPacket(LocoPacket);
    void newPacketDescription(QString);
    void trainUpdated(LocoTrain);
    void blockUpdated(LocoBlock);

protected:
    void set_timers();
    void unset_timers();
    QString handle_E7(LocoPacket _packet);
    QString handle_B2(LocoPacket _packet);
    QString handle_85(LocoPacket _packet);
    QString handle_83(LocoPacket _packet);
    QString handle_82(LocoPacket _packet);
    QString handle_81(LocoPacket _packet);
    QString handle_BF(LocoPacket _packet);
    QString handle_BD(LocoPacket _packet);
    QString handle_BC(LocoPacket _packet);
    QString handle_BB(LocoPacket _packet);
    QString handle_BA(LocoPacket _packet);
    QString handle_B9(LocoPacket _packet);
    QString handle_B8(LocoPacket _packet);
    QString handle_B6(LocoPacket _packet);
    QString handle_B5(LocoPacket _packet);
    QString handle_B4(LocoPacket _packet);
    QString handle_B1(LocoPacket _packet);
    QString handle_B0(LocoPacket _packet);
    QString handle_A2(LocoPacket _packet);
    QString handle_A1(LocoPacket _packet);
    QString handle_A0(LocoPacket _packet);

protected slots:
    void do_serialRead();

private:
    QVector<LocoTrain> trains;
    QVector<LocoBlock> blocks;
    QSerialPort * usbBuffer;
    LocoPacket incomingPacket;
    static bool debug;
    QTimer * trackTimer;
    QTimer * switchTimer;
    QTimer * trainTimer;
    int trackTimer_period, switchTimer_period, trainTimer_period;

};

#endif // LOCONET_H







