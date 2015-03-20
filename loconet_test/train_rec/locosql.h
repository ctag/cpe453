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

#include "locobyte.h"
#include "locoblock.h"

namespace std {
class LocoSQL;
}

class LocoSQL : public QObject
{
    Q_OBJECT

public:
    LocoSQL();
    ~LocoSQL();
    static bool debug;

public slots:
    bool do_openDB(QString hostname, int port, QString database, QString username, QString password);
    void do_closeDB();
    void do_updateBlock(LocoBlock _block);

signals:
    void DBopened();
    void DBclosed();

protected:

private:
    QSqlDatabase mainDB;
    QSqlQuery mainQuery;

};

#endif // LOCOSQL_H







