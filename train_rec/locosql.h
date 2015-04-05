#ifndef LOCOSQL_H
#define LOCOSQL_H

#include <QString>
#include <QDebug>
#include <QChar>
#include <QBitArray>
#include <QSqlDatabase>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>
#include <QPointer>

#include "locobyte.h"
#include "locoblock.h"
#include "locotrain.h"
#include "locopacket.h"

namespace std {
class LocoSQL;
}

class LocoSQL : public QObject
{
    Q_OBJECT

public:
    LocoSQL();
    ~LocoSQL();

public slots:
    bool do_openDB(QString hostname, int port, QString database, QString username, QString password);
    void do_closeDB();
    void do_updateBlock(LocoBlock _block);
    void do_updateTrain(LocoTrain _train);
    void do_updateSwitch(int _adr, bool _state);
    void do_clearTable(QString _table);
    void do_clearAllTables();
    void do_reqPacket();
    void do_reqTrain();
    void do_reqMacro();
    void do_reqSwitch();
    void do_cycleReqs();
    int get_percentFromHex(QString _hex);
    QString get_hexFromPercent(int _percent);
    QString get_hexFromInt(int _adr);
    void do_run();
    //void reqTimerReset();

signals:
    void DBopened();
    void DBclosed();
    void incomingRequest(LocoPacket _packet);
    void slotScan(LocoByte _slot);
    void slotDispatch(LocoByte _slot);
    void slotClear(LocoByte _slot);
    void slotReq(LocoByte _slot);
    void slotUse(LocoByte _slot);
    void trackReset();
    void trackOn();
    void trackOff();

protected:
    void reqTimerStop();
    void reqTimerStart(int _msec);

private:
    QSqlDatabase * mainDB;
    QSqlQuery * mainQuery;
    int * reqIndex;
    QPointer<QTimer> reqTimer;
    bool * debug;
    bool * doDelete;

};

#endif // LOCOSQL_H







