#ifndef SQL_H
#define SQL_H

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

#include "vertex.h"

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
    // Utility
    bool do_openDB(QString hostname, int port, QString database, QString username, QString password);
    void do_closeDB();
    void do_run();
    void do_clearTable(QString _table);
    void do_clearAllTables();
    // NodeToNode
    void do_clearVertices();
    void do_uploadVertex(vertex* _vert, QList<vertex*> _connected);
    // Requests and Listings
    void req_train(int _slot, int _speed, int _dir);
    void req_switch(int _adr, int _state);
    void req_slotScan(int _slot);
    void req_slotDispatch(int _slot);
    void req_slotClear(int _slot);
    void req_slotReq(int _train);
    void req_slotUse(int _slot);
    void req_trackReset();
    void req_trackOn();
    void req_trackOff();
    void do_listTrains();
    void do_listBlocks();

signals:
    void DBopened();
    void DBclosed();
    void trainsList(QVector<int> _adrs, QVector<int> _slots, QVector<int> _speeds, QVector<int> _dirs, QVector<QString> _states);
    void blocksList(QVector<QString> _ids, QVector<int> _states);

    /**
     * @brief mainDB
     * I've only been able to use pointers with sub-threads.
     * Pointers are assigned memory after the element has been instantiated
     * to avoid improper association with parent thread.
     */
private:
    QSqlDatabase * mainDB;
    QSqlQuery * mainQuery;
    bool * debug;

};

#endif // SQL_H







