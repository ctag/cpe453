#include "sql.h"

/**
 * @brief SQL Class
 * This class is designed to be used as a sub-thread to a main QT program.
 * To use it, implement it similarly as shown here, and emit signals from
 * the main application when you wish to operate on the SQL tables.
 */

/**
 * @brief SQL Consts
 * These constants allow for rapid tuning of SQL queries.
 * Backticks are used to ensure proper parsing of SQL statement,
 * remove them as you see fit.
 */
const QString schema = "`cpe453`"; // Schema for all tables.
const QString reqMacro = "`req_macro`"; // Table name for macro requests.
const QString reqTrain = "`req_train`"; // Table name for train/slot requests.
const QString reqSwitch = "`req_switch`"; // Table name for switch position requests.
const QString reqPacket = "`req_packet`"; // Table name for raw packet requests.
const QString trackTrain = "`track_train`"; // Table name for list of scanned trains/slots.
const QString trackBlock = "`track_ds`"; // Table name for list of Detection Sections
const QString trackVertices = "`track_vertices`"; // Table name for list of Detection Sections
const QString trackEdges = "`track_edges`"; // Table name for list of Detection Sections

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
    // Set database parameters handed over from the GUI
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

    // Apparently QSqlQuery objects shouldn't exist ouside of a connected database's scope..
    mainQuery = new QSqlQuery;
    *mainQuery = QSqlQuery(*mainDB);

    emit DBopened(); // Phone home with the good news.
    return(true); // Currently unimplemented.
}

void SQL::do_closeDB()
{
    if (mainDB == NULL)
    {
        return; // Nothing to be done?
    }
    mainDB->close();
    delete mainQuery;
    emit DBclosed();
}

/*
 * NodeToNode Specific SQL methods
 */

void SQL::do_clearVertices()
{
    do_clearTable("track_vertices");
    do_clearTable("track_edges");
}

void SQL::do_uploadVertex(vertex *_vert, QList<vertex *> _connected)
{
    if (*debug) qDebug() << "Uploading vertex.";
    if (!mainDB) {
        return;
    }
    if (!mainDB->isOpen())
    {
        return;
    }
    mainQuery->prepare("INSERT INTO "+schema+"."+trackVertices+" (`id`,`x`,`y`)"
                       "VALUES (:id,:x,:y);");
    mainQuery->bindValue(":id", _vert->vertexID);
    mainQuery->bindValue(":x", _vert->xposition);
    mainQuery->bindValue(":y", _vert->yposition);
    mainQuery->exec();

    for (int _index = 0; _index < _connected.count(); ++_index)
    {
        // TODO: add type and ds once available
        int edge_id = (_vert->vertexID * 10) + _index;
        mainQuery->prepare("INSERT INTO "+schema+"."+trackEdges+" (`edge_id`,`vert_from`,`vert_to`, `type`, `ds`)"
                           "VALUES (:edge_id,:vert_from,:vert_to,'0','0-0');");
        mainQuery->bindValue(":edge_id", edge_id);
        mainQuery->bindValue(":vert_from", _vert->vertexID);
        mainQuery->bindValue(":vert_to", _connected.at(_index)->vertexID);
        mainQuery->exec();
    }
}

void SQL::do_uploadEdges(QList<edge*> _edges)
{
    if (*debug) qDebug() << "Uploading edges.";
    if (!mainDB) {
        return;
    }
    if (!mainDB->isOpen())
    {
        return;
    }

    for (int _index = 0; _index < _edges.count(); ++_index)
    {
        edge * _edge = _edges.at(_index);
        mainQuery->prepare("INSERT INTO "+schema+"."+trackVertices+" (`id`,`x`,`y`)"
                           "VALUES (:id,:x,:y);");
        mainQuery->bindValue(":id", _edge->getVertFrom()->vertexID);
        mainQuery->bindValue(":x", _edge->getVertFrom()->xInch);
        mainQuery->bindValue(":y", _edge->getVertFrom()->yInch);
        mainQuery->exec();

        mainQuery->prepare("INSERT INTO "+schema+"."+trackVertices+" (`id`,`x`,`y`)"
                           "VALUES (:id,:x,:y);");
        mainQuery->bindValue(":id", _edge->getVertTo()->vertexID);
        mainQuery->bindValue(":x", _edge->getVertTo()->xInch);
        mainQuery->bindValue(":y", _edge->getVertTo()->yInch);
        mainQuery->exec();

        mainQuery->prepare("INSERT INTO "+schema+"."+trackEdges+" (`id`,`vert_from`,`vert_to`, `type`, `ds`)"
                           "VALUES (:edge_id,:vert_from,:vert_to,:type,:ds);");
        mainQuery->bindValue(":id", _index);
        mainQuery->bindValue(":vert_from", _edge->getVertFrom()->vertexID);
        mainQuery->bindValue(":vert_to", _edge->getVertTo()->vertexID);
        mainQuery->bindValue(":type", _edge->getType());
        mainQuery->bindValue(":ds", _edge->getDS());
        mainQuery->exec();
    }
}

/*
 * General SQL methods
 */

void SQL::req_trackReset()
{
    if (*debug) qDebug() << "Requesting track reset.";
    if (!mainDB) {
        return;
    }
    if (!mainDB->isOpen())
    {
        return;
    }
    mainQuery->prepare("INSERT INTO "+schema+"."+reqMacro+" (`macro`)"
                       "VALUES ('TRACK_RESET');");
    mainQuery->exec();
}

void SQL::req_trackOn()
{
    if (*debug) qDebug() << "Requesting track On.";
    if (!mainDB) {
        return;
    }
    if (!mainDB->isOpen())
    {
        return;
    }
    mainQuery->prepare("INSERT INTO "+schema+"."+reqMacro+" (`macro`)"
                       "VALUES ('TRACK_ON');");
    mainQuery->exec();
}

void SQL::req_trackOff()
{
    if (*debug) qDebug() << "Requesting track off.";
    if (!mainDB) {
        return;
    }
    if (!mainDB->isOpen())
    {
        return;
    }
    mainQuery->prepare("INSERT INTO "+schema+"."+reqMacro+" (`macro`)"
                       "VALUES ('TRACK_OFF');");
    mainQuery->exec();
}

void SQL::req_slotReq(int _train)
{
    if (*debug) qDebug() << "Requesting slot for train" << _train;
    if (!mainDB) {
        return;
    }
    if (!mainDB->isOpen())
    {
        return;
    }
    mainQuery->prepare("INSERT INTO "+schema+"."+reqMacro+" (`macro`, `arg1`)"
                       "VALUES ('SLOT_REQ', '"+QString::number(_train)+"');");
    mainQuery->exec();
}

void SQL::req_slotUse(int _slot)
{
    if (*debug) qDebug() << "Requesting slot for train " << _slot;
    if (!mainDB) {
        return;
    }
    if (!mainDB->isOpen())
    {
        return;
    }
    mainQuery->prepare("INSERT INTO "+schema+"."+reqMacro+" (`macro`, `arg1`)"
                       "VALUES ('SLOT_USE', '"+QString::number(_slot)+"');");
    mainQuery->exec();
}

void SQL::req_slotScan(int _slot)
{
    if (*debug) qDebug() << "Requesting information about slot " << _slot;
    if (!mainDB) {
        return;
    }
    if (!mainDB->isOpen())
    {
        return;
    }
    mainQuery->prepare("INSERT INTO "+schema+"."+reqMacro+" (`macro`, `arg1`)"
                       "VALUES ('SLOT_SCAN', '"+QString::number(_slot)+"');");
    mainQuery->exec();
}

void SQL::req_slotClear(int _slot)
{
    if (*debug) qDebug() << "Requesting slot be cleared " << _slot;
    if (!mainDB) {
        return;
    }
    if (!mainDB->isOpen())
    {
        return;
    }
    mainQuery->prepare("INSERT INTO "+schema+"."+reqMacro+" (`macro`, `arg1`)"
                       "VALUES ('SLOT_CLEAR', '"+QString::number(_slot)+"');");
    mainQuery->exec();
}

void SQL::req_slotDispatch(int _slot)
{
    if (*debug) qDebug() << "Requesting dispatch of slot " << _slot;
    if (!mainDB) {
        return;
    }
    if (!mainDB->isOpen())
    {
        return;
    }
    mainQuery->prepare("INSERT INTO "+schema+"."+reqMacro+" (`macro`, `arg1`)"
                       "VALUES ('SLOT_DISPATCH', '"+QString::number(_slot)+"');");
    mainQuery->exec();
}

void SQL::req_train(int _slot, int _speed, int _dir)
{
    if (*debug) qDebug() << "Requesting dispatch of slot " << _slot << _speed << _dir;
    if (!mainDB) {
        return;
    }
    if (!mainDB->isOpen())
    {
        return;
    }
    mainQuery->prepare("INSERT INTO "+schema+"."+reqTrain+" (`slot`, `speed`, `dir`)"
                       "VALUES (:slot, :speed, :dir);");
    mainQuery->bindValue(":slot", _slot);
    mainQuery->bindValue(":speed", _speed);
    mainQuery->bindValue(":dir", _dir);
    mainQuery->exec();
}

void SQL::do_listTrains()
{
    if (*debug) qDebug() << "Requesting list of all trains.";
    if (!mainDB) {
        return;
    }
    if (!mainDB->isOpen())
    {
        return;
    }
    mainQuery->prepare("SELECT * FROM "+schema+"."+trackTrain+";");
    mainQuery->exec();
    QVector<int> _adrs, _slots, _speeds, _dirs;
    QVector<QString> _states;
    while (mainQuery->next())
    {
        qDebug() << "train found in SQL: " << mainQuery->value("slot").toInt();
        _adrs.append(mainQuery->value("adr").toInt());
        _slots.append(mainQuery->value("slot").toInt());
        _speeds.append(mainQuery->value("speed").toInt());
        _dirs.append(mainQuery->value("dir").toInt());
        _states.append(mainQuery->value("state").toString());
    }
    emit trainsList(_adrs, _slots, _speeds, _dirs, _states);
}

void SQL::do_listBlocks()
{
    if (*debug) qDebug() << "Requesting list of all blocks.";
    if (!mainDB) {
        return;
    }
    if (!mainDB->isOpen())
    {
        return;
    }
    mainQuery->prepare("SELECT * FROM "+schema+"."+trackBlock+" ORDER BY `status` DESC, `ds_id` ASC;");
    mainQuery->exec();
    QVector<QString> _ids;
    QVector<int> _positions;
    while (mainQuery->next())
    {
        qDebug() << "block found in SQL: " << mainQuery->value("ds_id").toInt();
        _ids.append(mainQuery->value("ds_id").toString());
        _positions.append(mainQuery->value("status").toInt());
    }
    emit blocksList(_ids, _positions);
}

void SQL::req_switch(int _adr, int _state)
{
    if (*debug) qDebug() << "Requesting new state for switch " << _adr;
    if (!mainDB) {
        return;
    }
    if (!mainDB->isOpen())
    {
        return;
    }
    mainQuery->prepare("INSERT INTO "+schema+"."+reqSwitch+" (`id`, `position`)"
                       "VALUES (:adr, :state);");
    mainQuery->bindValue(":adr", _adr);
    mainQuery->bindValue(":state", _state);
    mainQuery->exec();
}

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
