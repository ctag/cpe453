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
    void do_clearTable(QString _table);
    void do_clearAllTables();
    void do_reqPacket();
    void do_reqTrain();
    void do_reqMacro();
    void do_cycleReqs();
    int get_percentFromHex(QString _hex);
    QString get_hexFromPercent(int _percent);
    QString get_hexFromInt(int _adr);
    void run();

signals:
    void DBopened();
    void DBclosed();
    void incomingRequest(LocoPacket _packet);

protected:
    void reqTimerStop();
    void reqTimerStart(int _msec);

private:
    QSqlDatabase * mainDB;
    QSqlQuery * mainQuery;
    //int reqDelay;
    int * reqIndex;
    QTimer * reqTimer;
    bool debug;

};

#endif // LOCOSQL_H







