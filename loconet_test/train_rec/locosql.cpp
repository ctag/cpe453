#include "locosql.h"

bool LocoSQL::debug = true;

LocoSQL::LocoSQL()
{
    mainDB = QSqlDatabase::addDatabase("QMYSQL", "main");
    mainQuery = QSqlQuery(mainDB);
    reqDelay = 10000;
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
    do_cycleReqs();

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

int LocoSQL::get_percentFromHex(QString _hex) {
    int _percent = _hex.toInt(0, 16);
    _percent *= 0.787401575;
    if (_percent < 0) {
        _percent = 0;
    } else if (_percent > 100) {
        _percent = 100;
    }
    qDebug() << "GET PERCENT FROM HEX: " << _percent;
    return(_percent);
}

QString LocoSQL::get_hexFromPercent(int _percent) {
    if (_percent < 2) {
        return("00");
    }
    _percent *= 1.27;
    QString _hex =  QString::number(_percent, 16);
    qDebug() << "GET HEX FROM PERCENT: " << _hex;
    return(_hex);
}

QString LocoSQL::get_hexFromInt(int _adr) {
    QString _hex =  QString("%1").arg(_adr, 2, 16, QChar('0'));//QString::number(_adr, 16);
    qDebug() << "GET HEX FROM ADDRESS: " << _hex;
    return(_hex);
}

void LocoSQL::do_cycleReqs()
{
    switch(reqIndex)
    {
    case 0:
        do_reqTrain();
        reqIndex = 1;
        break;
    case 1:
        do_reqPacket();
        reqIndex = 0;
        break;
    default:
        reqIndex = 0;
        break;
    }
    QTimer::singleShot(reqDelay, this, SLOT(do_cycleReqs()));
}

void LocoSQL::do_reqTrain()
{
    if (debug) qDebug() << "Querying for throttle requests.";
    if (!mainDB.isOpen())
    {
        // open
        return;
    }
    mainQuery.prepare("SELECT * FROM cpe453.req_train;");
    mainQuery.exec();
    while (mainQuery.next())
    {
        if (debug) qDebug() << "TESTING SQL " << mainQuery.value("id").toString() << ":" << mainQuery.value("speed").toString();
        get_percentFromHex(mainQuery.value("speed").toString());
        get_hexFromPercent(get_percentFromHex(mainQuery.value("speed").toString()));
        get_hexFromInt(mainQuery.value("id").toInt());
        //continue;
        LocoByte _command;
        LocoPacket _packet;
        LocoByte _slot;
        LocoByte _speed;
        _command.set_fromHex("A0");
        _speed.set_fromHex(get_hexFromPercent(mainQuery.value("speed").toInt()));
        _slot.set_fromHex(get_hexFromInt(mainQuery.value("id").toInt()));
        _packet.do_appendLocoByte(_command);
        _packet.do_appendLocoByte(_slot);
        _packet.do_appendLocoByte(_speed);
        _packet.do_genChecksum();
        emit incomingRequest(_packet);
        if (debug) qDebug() << "Found throttle request. " << _packet.get_packet();
    }
    //QTimer::singleShot(reqDelay, this, SLOT(do_reqTrain()));
}

void LocoSQL::do_reqPacket()
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
    //QTimer::singleShot(reqDelay, this, SLOT(do_reqPacket()));
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
        int _speed = get_percentFromHex(_train.get_speed().get_hex());
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

