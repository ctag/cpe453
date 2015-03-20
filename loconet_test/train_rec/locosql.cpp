#include "locosql.h"

bool LocoSQL::debug = true;

LocoSQL::LocoSQL()
{
    mainDB = QSqlDatabase::addDatabase("QMYSQL", "main");
    mainQuery = QSqlQuery(mainDB);
}

LocoSQL::~LocoSQL()
{
    //
}

bool LocoSQL::do_openDB(QString hostname, int port, QString database, QString username, QString password)
{
    mainDB.setHostName(hostname);
    mainDB.setPort(port);
    mainDB.setDatabaseName(database);
    mainDB.setUserName(username);
    mainDB.setPassword(password);

    // Attempt to open database
    if (!mainDB.open())
    {
        qDebug() << "Opening postgresql database failed D:";
        qDebug() << mainDB.lastError();
        return(false);
    }
    emit DBopened();
    return(true);
}

void LocoSQL::do_closeDB()
{
    mainDB.close();
    emit DBclosed();
}

void LocoSQL::do_updateBlock(LocoBlock _block)
{
    if (mainDB.isOpen()) {
        mainQuery.prepare("INSERT INTO cpe453.track_ds (ds_id, status) "
                        "VALUES (:id, :status) "
                        "ON DUPLICATE KEY "
                        "UPDATE ds_id=:id, status=:status;");
        QString _id = QString::fromLatin1(_block.get_adr());
        bool _status = _block.get_occupied();
        mainQuery.bindValue(":id", _id);
        mainQuery.bindValue(":status", _status);
        if (debug) qDebug() << "Updating SQL block: ";
        mainQuery.exec();
    }
}


