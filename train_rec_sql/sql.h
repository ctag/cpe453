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
    void do_reqTrain(int _slot, int _speed, int _dir);
    void do_listTrains();
    void do_listBlocks();
    /*void do_reqMacro(QString _macro);
    void do_reqMacro1(QString _macro, QString _arg1);*/
    void do_reqSwitch(int _adr, int _state);
    void do_slotScan(int _slot);
    void do_slotDispatch(int _slot);
    void do_slotClear(int _slot);
    void do_slotReq(int _train);
    void do_slotUse(int _slot);
    void do_trackReset();
    void do_trackOn();
    void do_trackOff();
    void do_run();

signals:
    void DBopened();
    void DBclosed();
    void trainList(QVector<int> _adrs, QVector<int> _slots, QVector<int> _speeds, QVector<int> _dirs, QVector<QString> _states);
    void blockList(QVector<QString> _ids, QVector<int> _states);

protected:
    void reqTimerStop();
    void reqTimerStart(int _msec);

private:
    QSqlDatabase * mainDB;
    QSqlQuery * mainQuery;
    bool * debug;

};

#endif // LOCOSQL_H







