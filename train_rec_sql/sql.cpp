#include "sql.h"

const QString schema = "`cpe453`";
const QString macro = "`req_macro`";

SQL::SQL()
{
    debug = NULL;
    mainDB = NULL;
    mainQuery = NULL;
}

SQL::~SQL()
{
    if (mainDB != NULL)
    {
        mainDB->close();
        delete mainDB;
        delete mainQuery;
    }
    delete debug;
}

void SQL::do_run()
{
    qDebug() << "Initializing SQL thread.";
    mainDB = new QSqlDatabase;
    *mainDB = QSqlDatabase::addDatabase("QMYSQL", "main");
    debug = new bool;
    *debug = true;
}

bool SQL::do_openDB(QString hostname, int port, QString database, QString username, QString password)
{
    mainQuery = new QSqlQuery;
    *mainQuery = QSqlQuery(*mainDB);

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

    emit DBopened();
    return(true);
}

void SQL::do_closeDB()
{
    mainDB->close();
    delete mainQuery;
    emit DBclosed();
}

void SQL::do_trackReset()
{
    if (*debug) qDebug() << "Requesting track reset.";
    if (!mainDB) {
        return;
    }
    if (!mainDB->isOpen())
    {
        // open
        return;
    }
    mainQuery->prepare("INSERT INTO "+schema+"."+macro+" (`macro`)"
                       "VALUES ('TRACK_RESET');");
    mainQuery->exec();
}

void SQL::do_trackOn()
{
    if (*debug) qDebug() << "Requesting track On.";
    if (!mainDB) {
        return;
    }
    if (!mainDB->isOpen())
    {
        // open
        return;
    }
    mainQuery->prepare("INSERT INTO "+schema+"."+macro+" (`macro`)"
                       "VALUES ('TRACK_ON');");
    mainQuery->exec();
}

void SQL::do_trackOff()
{
    if (*debug) qDebug() << "Requesting track off.";
    if (!mainDB) {
        return;
    }
    if (!mainDB->isOpen())
    {
        // open
        return;
    }
    mainQuery->prepare("INSERT INTO "+schema+"."+macro+" (`macro`)"
                       "VALUES ('TRACK_OFF');");
    mainQuery->exec();
}

/*
void SQL::reqTimerStart(int _msec)
{
    if (reqTimer)
    {
        return;
    }
    reqTimer = new QTimer;
    connect(reqTimer, SIGNAL(timeout()), this, SLOT(do_cycleReqs()));
    reqTimer->start(_msec);
}

void SQL::reqTimerStop()
{
    if (!reqTimer) {
        return;
    }
    disconnect(reqTimer, 0, 0, 0);
    reqTimer->stop();
    reqTimer->deleteLater();
}
*/

void SQL::do_clearAllTables()
{
    do_clearTable("track_ds");
    do_clearTable("track_train");
}

void SQL::do_clearTable(QString _table)
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
/*
void SQL::do_reqMacro()
{
    if (*debug) qDebug() << "Querying for macro request.";
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
    if (mainQuery->next())
    {
    }
}

void SQL::do_reqSwitch()
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
    if (mainQuery->next())
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

void SQL::do_reqTrain()
{
    if (*debug) qDebug() << "Querying for train requests.";
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
    if (mainQuery->next())
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

void SQL::do_reqPacket()
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
    mainQuery->prepare("SELECT * FROM cpe453.req_packet;");
    mainQuery->exec();
    if (mainQuery->next())
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
*/

/*
 * Status updating methods
 */

/*
void SQL::do_updateBlock(LocoBlock _block)
{
    if (!mainDB) {
        return;
    }
    if (mainDB->isOpen()) {
        mainQuery->prepare("INSERT INTO cpe453.track_ds (ds_id, status) "
                        "VALUES (:id, :status) "
                        "ON DUPLICATE KEY "
                        "UPDATE status=:status;");
        QString _id = QString::fromLatin1(_block.get_adr());
        int _status = _block.get_occupied();
        mainQuery->bindValue(":id", _id);
        mainQuery->bindValue(":status", _status);
        if (*debug) qDebug() << "Updating SQL block: " << _id << ":" << _status;
        mainQuery->exec();
    }
}

void SQL::do_updateTrain (LocoTrain _train)
{
    if (!mainDB) {
        return;
    }
    if (mainDB->open()) {
        QString _adr = _train.get_adr().get_hex();
        QString _slot = _train.get_slot().get_hex();
        int _speed = get_percentFromHex(_train.get_speed().get_hex());
        int _dir = _train.get_direction()?1:0;
        QString _state = _train.get_state();

        mainQuery->prepare("INSERT INTO cpe453.track_train (slot, adr, speed, dir, state) "
                          "VALUES (:slot, :adr, :speed, :dir, :state) "
                          "ON DUPLICATE KEY "
                          "UPDATE adr=:adr, speed=:speed, dir=:dir, state=:state;");
        mainQuery->bindValue(":slot", _slot);
        mainQuery->bindValue(":adr", _adr);
        mainQuery->bindValue(":speed", _speed);
        mainQuery->bindValue(":dir", _dir);
        mainQuery->bindValue(":state", _state);
        if (*debug) qDebug() << "Updating train SQL." << _slot << ":" << _speed;
        mainQuery->exec();
    }
}

void SQL::do_updateSwitch(int _adr, bool _state)
{
    if (!mainDB) {
        return;
    }
    if (mainDB->open()) {
        //QString _address = QString::number(_adr);

        mainQuery->prepare("INSERT INTO cpe453.track_switch (adr, state) "
                          "VALUES (:adr, :state) "
                          "ON DUPLICATE KEY "
                          "UPDATE state=:state;");
        mainQuery->bindValue(":adr", _adr);
        mainQuery->bindValue(":state", _state);
        if (*debug) qDebug() << "Updating switch SQL." << _adr << ":" << _state;
        mainQuery->exec();
        //
    }
}
*/

