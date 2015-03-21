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
    // Clear status tables on startup
    do_clearAllTables();

    emit DBopened();
    return(true);
}

void LocoSQL::do_closeDB()
{
    mainDB.close();
    emit DBclosed();
}

void LocoSQL::do_clearAllTables()
{
    do_clearTable("track_ds");
    do_clearTable("track_trains");
}

void LocoSQL::do_clearTable(QString _table)
{
    if (mainDB.isOpen()) {
        _table.prepend("DELETE FROM cpe453.");
        mainQuery.prepare(_table);
        mainQuery.bindValue(":table", _table);
        if (debug) qDebug() << "Deleting all rows in SQL cpe453." << _table;
        if (!mainQuery.exec())
        {
            qDebug() << mainQuery.lastError();
        }
    }
}

void LocoSQL::do_findReqs()
{
    if (!mainDB.isOpen())
    {
        // open
        return;
    }
    mainQuery.prepare("SELECT packet FROM cpe453.req_packets;");
    mainQuery.exec();
    while (mainQuery.next())
    {
        LocoPacket _packet(mainQuery.value("packet").toString());
        if (_packet.validOP())
        {
            emit incomingRequest(_packet);
        }
    }
    QTimer::singleShot(200, this, SLOT(do_findReqs()));
}

/*
 * Status updating methods
 */

/**
 * @brief LocoSQL::do_updateBlock
 * @param _block
 */
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

/**
 * @brief LocoSQL::do_updateTrain
 * @param _train
 */
void LocoSQL::do_updateTrain (LocoTrain _train)
{
    if (mainDB.open()) {
        QString _adr = _train.get_adr().get_hex();
        QString _slot = _train.get_slot().get_hex();
        int _speed = _train.get_speed().get_decimal();
        int _dir = _train.get_direction()?1:0;

        mainQuery.prepare("INSERT INTO cpe453.track_trains (slot, adr, speed, dir) "
                          "VALUES (:slot, :adr, :speed, :dir) "
                          "ON DUPLICATE KEY "
                          "UPDATE adr=:adr, speed=:speed, dir=:dir;");
        mainQuery.bindValue(":slot", _slot);
        mainQuery.bindValue(":adr", _adr);
        mainQuery.bindValue(":speed", _speed);
        mainQuery.bindValue(":dir", _dir);
        if (debug) qDebug() << "Updating train SQL.";
        mainQuery.exec();
    }
}

