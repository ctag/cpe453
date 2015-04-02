#include "locosql.h"

LocoSQL::LocoSQL()
{
    debug = NULL;
    doDelete = NULL;
    mainDB = NULL;
    mainQuery = NULL;
    reqIndex = NULL;
}

LocoSQL::~LocoSQL()
{
    delete reqTimer;
    mainDB->close();
    delete mainDB;
    delete mainQuery;
    delete debug;
    delete doDelete;
}

void LocoSQL::run()
{
    qDebug() << "Initializing SQL thread.";
    mainDB = new QSqlDatabase;
    *mainDB = QSqlDatabase::addDatabase("QMYSQL", "main");
    mainQuery = new QSqlQuery;
    *mainQuery = QSqlQuery(*mainDB);
    reqIndex = new int;
    debug = new bool;
    *debug = false;
    doDelete = new bool;
    *doDelete = true;
}

bool LocoSQL::do_openDB(QString hostname, int port, QString database, QString username, QString password)
{
    mainDB->setHostName(hostname);
    mainDB->setPort(port);
    mainDB->setDatabaseName(database);
    mainDB->setUserName(username);
    mainDB->setPassword(password);

    // Attempt to open database
    if (!mainDB->open())
    {
        qDebug() << "Opening postgresql database failed D:";
        qDebug() << mainDB->lastError();
        return(false);
    }
    // Clear status tables on startup
    if (*doDelete) {
        do_clearAllTables();
    }
    reqTimerStart(400);

    emit DBopened();
    return(true);
}

void LocoSQL::do_closeDB()
{
    reqTimerStop();
    mainDB->close();
    emit DBclosed();
}

void LocoSQL::reqTimerStart(int _msec)
{
    reqTimer = new QTimer;
    connect(reqTimer, SIGNAL(timeout()), this, SLOT(do_cycleReqs()));
    reqTimer->start(_msec);
}

void LocoSQL::reqTimerStop()
{
    disconnect(reqTimer, 0, 0, 0);
    reqTimer->stop();
    delete reqTimer;
}

void LocoSQL::do_clearAllTables()
{
    do_clearTable("track_ds");
    do_clearTable("track_train");
}

void LocoSQL::do_clearTable(QString _table)
{
    if (!mainDB) {
        return;
    }
    if (mainDB->isOpen()) {
        _table.prepend("DELETE FROM cpe453.");
        mainQuery->prepare(_table);
        mainQuery->bindValue(":table", _table);
        if (*debug) qDebug() << "Deleting all rows in SQL cpe453." << _table;
        if (!mainQuery->exec())
        {
            qDebug() << mainQuery->lastError();
        }
    }
}

int LocoSQL::get_percentFromHex(QString _hex) {
    if (*debug) qDebug() << "percentfromhex: " << _hex;
    int _percent = _hex.toInt(0, 16);
    _percent = _percent*0.8;
    if (_percent < 0) {
        _percent = 0;
    } else if (_percent > 100) {
        _percent = 100;
    }
    if (*debug) qDebug() << "get_percentFromHex(): " << _percent;
    return(_percent);
}

QString LocoSQL::get_hexFromPercent(int _percent) {
    if (_percent < 2) {
        return("00");
    }
    _percent = _percent*1.25;
    if (*debug) qDebug() << "Percent " << _percent;
    QString _hex =  QString("%1").arg(_percent, 2, 16, QChar('0')); //QString::number(_percent, 16);
    if (*debug) qDebug() << "get_hexFromPercent(): " << _hex;
    get_percentFromHex(_hex);
    return(_hex);
}

QString LocoSQL::get_hexFromInt(int _adr) {
    QString _hex =  QString("%1").arg(_adr, 2, 16, QChar('0'));//QString::number(_adr, 16);
    if (*debug) qDebug() << "get_hexFromInt(): " << _hex;
    return(_hex);
}

void LocoSQL::do_cycleReqs()
{
    do_reqMacro();
    return;
    switch(*reqIndex)
    {
    case 0:
        do_reqTrain();
        *reqIndex = 1;
        break;
    case 1:
        do_reqPacket();
        *reqIndex = 2;
        break;
    case 2:
        do_reqSwitch();
        *reqIndex = 3;
        break;
    case 3:
        do_reqMacro();
        *reqIndex = 0;
        break;
    default:
        *reqIndex = 0;
        break;
    }
}

void LocoSQL::do_reqMacro()
{
    /*if (*debug)*/ qDebug() << "Querying for macro requests.";
    if (!mainDB) {
        return;
    }
    if (!mainDB->isOpen())
    {
        // open
        return;
    }
    mainQuery->prepare("SELECT * FROM cpe453.req_macro;");
    mainQuery->exec();
    while (mainQuery->next())
    {
        QString _macro = mainQuery->value("macro").toString();
        int _id = mainQuery->value("id").toInt();
        if (_macro == "SCAN_TRAINS") {
            /*if (debug)*/ qDebug() << "Scanning for trains";
            emit scanTrains();
        }
        if (*doDelete)
        {
            /*if (debug)*/ qDebug() << "deleting macro id: " << _id;
            mainQuery->prepare("DELETE FROM cpe453.req_macro WHERE id=:_id LIMIT 1;");
            mainQuery->bindValue(":_id", _id);
            mainQuery->exec();
        }
    }
}

void LocoSQL::do_reqSwitch()
{
    if (*debug) qDebug() << "Querying for switch requests.";
    if (!mainDB) {
        return;
    }
    if (!mainDB->isOpen())
    {
        // open
        return;
    }
    mainQuery->prepare("SELECT * FROM cpe453.req_switch;");
    mainQuery->exec();
    while (mainQuery->next())
    {
        LocoPacket _packet;
        LocoByte _command;
        LocoByte _id;
        int _idInt;
        LocoByte _position;
        int _positionInt;
        _command.set_fromHex("B0");
        _idInt = mainQuery->value("id").toInt();
        _id.set_fromHex(get_hexFromInt(_idInt-1));
        _positionInt = mainQuery->value("position").toInt();
        _position.set_fromHex(get_hexFromInt((((_positionInt)*2)+1)*16));
        _packet.do_appendLocoByte(_command);
        _packet.do_appendLocoByte(_id);
        _packet.do_appendLocoByte(_position);
        _packet.do_genChecksum();
        if (*debug) qDebug() << "Found switch request: " << _packet.get_packet();
        emit incomingRequest(_packet);
        if (*doDelete)
        {
            mainQuery->prepare("DELETE FROM cpe453.req_switch WHERE id=:_id LIMIT 1;");
            mainQuery->bindValue(":_id", _idInt);
            mainQuery->exec();
        }
    }
}

void LocoSQL::do_reqTrain()
{
    if (*debug) qDebug() << "Querying for throttle requests.";
    if (!mainDB) {
        return;
    }
    if (!mainDB->isOpen())
    {
        // open
        return;
    }
    mainQuery->prepare("SELECT * FROM cpe453.req_train;");
    mainQuery->exec();
    while (mainQuery->next())
    {
        LocoByte _command;
        LocoPacket _speedPacket;
        LocoPacket _dirPacket;
        LocoByte _slot;
        int _slotInt;
        LocoByte _speed;
        int _dir;
        _command.set_fromHex("A0");
        _speed.set_fromHex(get_hexFromPercent(mainQuery->value("speed").toInt()));
        _slotInt = mainQuery->value("slot").toInt();
        _slot.set_fromHex(get_hexFromInt(_slotInt));
        _dir = mainQuery->value("dir").toBool();
        _speedPacket.do_appendLocoByte(_command);
        _speedPacket.do_appendLocoByte(_slot);
        _speedPacket.do_appendLocoByte(_speed);
        _speedPacket.do_genChecksum();
        emit incomingRequest(_speedPacket);
        if (*debug) qDebug() << "Found speed request. " << _speedPacket.get_packet();
        _command.set_fromHex("A1");
        _dirPacket.do_appendLocoByte(_command);
        _dirPacket.do_appendLocoByte(_slot);
        _dirPacket.do_appendByte(QString::number(((_dir)?2:0)+1)+"0");
        emit incomingRequest(_dirPacket);
        if (*debug) qDebug() << "Found dir request. " << _dirPacket.get_packet();
        if (*doDelete)
        {
            mainQuery->prepare("DELETE FROM cpe453.req_train WHERE slot=:_slot LIMIT 1;");
            mainQuery->bindValue(":_slot", _slotInt);
            mainQuery->exec();
        }
    }
}

void LocoSQL::do_reqPacket()
{
    if (*debug) qDebug() << "Querying for packet requests.";
    if (!mainDB) {
        return;
    }
    if (!mainDB->isOpen())
    {
        // open
        return;
    }
    mainQuery->prepare("SELECT packet FROM cpe453.req_packet;");
    mainQuery->exec();
    while (mainQuery->next())
    {
        LocoPacket _packet(mainQuery->value("packet").toString());
        int _id = mainQuery->value("id").toInt();
        if (_packet.validOP())
        {
            if (*debug) qDebug() << "Found packet request: " << _packet.get_packet();
            emit incomingRequest(_packet);
        }
        if (*doDelete)
        {
            mainQuery->prepare("DELETE FROM cpe453.req_packet WHERE id=:_id LIMIT 1;");
            mainQuery->bindValue(":_id", _id);
            mainQuery->exec();
        }
    }
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
    if (!mainDB) {
        return;
    }
    if (mainDB->isOpen()) {
        mainQuery->prepare("INSERT INTO cpe453.track_ds (ds_id, status) "
                        "VALUES (:id, :status) "
                        "ON DUPLICATE KEY "
                        "UPDATE ds_id=:id, status=:status;");
        QString _id = QString::fromLatin1(_block.get_adr());
        bool _status = _block.get_occupied();
        mainQuery->bindValue(":id", _id);
        mainQuery->bindValue(":status", _status);
        if (*debug) qDebug() << "Updating SQL block: ";
        mainQuery->exec();
    }
}

/**
 * @brief LocoSQL::do_updateTrain
 * @param _train
 */
void LocoSQL::do_updateTrain (LocoTrain _train)
{
    if (!mainDB) {
        return;
    }
    if (mainDB->open()) {
        QString _adr = _train.get_adr().get_hex();
        QString _slot = _train.get_slot().get_hex();
        int _speed = get_percentFromHex(_train.get_speed().get_hex());
        int _dir = _train.get_direction()?1:0;

        mainQuery->prepare("INSERT INTO cpe453.track_train (slot, adr, speed, dir) "
                          "VALUES (:slot, :adr, :speed, :dir) "
                          "ON DUPLICATE KEY "
                          "UPDATE adr=:adr, speed=:speed, dir=:dir;");
        mainQuery->bindValue(":slot", _slot);
        mainQuery->bindValue(":adr", _adr);
        mainQuery->bindValue(":speed", _speed);
        mainQuery->bindValue(":dir", _dir);
        if (*debug) qDebug() << "Updating train SQL.";
        mainQuery->exec();
    }
}

