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

namespace std {
class SQL;
}

class SQL : public QObject
{
    Q_OBJECT

public:
    SQL();
    ~SQL();

public slots:
    bool do_openDB(QString hostname, int port, QString database, QString username, QString password);
    void do_closeDB();
    void do_clearTable(QString _table);
    void do_clearAllTables();
    //void do_reqTrain(int _slot, int _speed, int _dir);
    /*void do_reqMacro(QString _macro);
    void do_reqMacro1(QString _macro, QString _arg1);
    void do_reqSwitch(int _adr, int _state);
    void do_incomingRequest(LocoPacket _packet);
    void do_slotScan(LocoByte _slot);
    void do_slotDispatch(LocoByte _slot);
    void do_slotClear(LocoByte _slot);
    void do_slotReq(LocoByte _slot);
    void do_slotUse(LocoByte _slot);*/
    void do_trackReset();
    void do_trackOn();
    void do_trackOff();
    void do_run();

signals:
    void DBopened();
    void DBclosed();

protected:
    void reqTimerStop();
    void reqTimerStart(int _msec);

private:
    QSqlDatabase * mainDB;
    QSqlQuery * mainQuery;
    bool * debug;

};

#endif // LOCOSQL_H







