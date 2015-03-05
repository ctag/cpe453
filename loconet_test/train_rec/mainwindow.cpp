#include "mainwindow.h"
#include "ui_mainwindow.h"

/* Conventions:
 *
 * Underscores indicate a local variable to a function
 *
 * set_ to change a member variable
 * get_ to retrieve a member variable
 * do_ to complete a task
 * is_ to query a state of the object
 * handle_ to take care of a signal
 */

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    outgoingPacket.set_allFromHex("00");
    do_initStaticOP(outgoingPacket);

    ui->lineEdit_opcode->setInputMask("hh");
    ui->lineEdit_arg1->setInputMask("hh");
    ui->lineEdit_arg2->setInputMask("hh");
    ui->lineEdit_opcode->setText("");

    connect(ui->pushButton_genPacket, SIGNAL(clicked()), this, SLOT(do_genPacket()));
    connect(ui->lineEdit_opcode, SIGNAL(returnPressed()), this, SLOT(do_enableArgs()));
    connect(ui->comboBox_opcodes, SIGNAL(currentIndexChanged(int)), this, SLOT(do_OPfromComboBox()));
    connect(ui->pushButton_serialRefreshList, SIGNAL(clicked()), this, SLOT(do_serialRefreshList()));
    connect(ui->pushButton_serialConnect, SIGNAL(clicked()), this, SLOT(do_serialConnect()));
    connect(ui->pushButton_serialDisconnect, SIGNAL(clicked()), this, SLOT(do_serialDisconnect()));
    connect(ui->pushButton_sendPacket, SIGNAL(clicked()), this, SLOT(sendSerial()));
    connect(&loconet, &LocoNet::newPacket, this, &MainWindow::displayPacket); // QT-5 style works
    connect(&loconet, &LocoNet::newPacketDescription, this, &MainWindow::printDescriptions);
    connect(&loconet, &LocoNet::trainUpdated, this, &MainWindow::updateTrains);
    connect(ui->comboBox_packetHistory, SIGNAL(activated(int)), this, SLOT(loadFromPacketHistory(int)));
    connect(ui->pushButton_resetTrack, SIGNAL(clicked()), this, SLOT(do_resetTrack()));

    connect(ui->pushButton_connect, SIGNAL(clicked()), this, SLOT(connectDB()));
    connect(ui->pushButton_disconnect, SIGNAL(clicked()), this, SLOT(disconnectDB()));
    //connect(ui->pushButton_tableText, SIGNAL(clicked()), this, SLOT(tableText()));
    //connect(ui->pushButton_queryModel, SIGNAL(clicked()), this, SLOT(queryModel()));
    connect(ui->pushButton_runQuery, SIGNAL(clicked()), this, SLOT(manualQuery()));

    ui->comboBox_opcodes->setEditable(false);
    ui->comboBox_opcodes->setInsertPolicy(QComboBox::InsertAtBottom);

    do_loadOPComboBox();
    do_serialRefreshList();

    packetTimer = new QTimer(this);
    connect(packetTimer, SIGNAL(timeout()), this, SLOT(do_packetTimer()));
    connect(ui->pushButton_timerToggle, SIGNAL(clicked()), this, SLOT(do_timerToggle()));

    db = QSqlDatabase::addDatabase("QPSQL", "main");
    dbQuery = QSqlQuery(db);

    // Added to speed up debugging process
    do_serialConnect();
    connectDB();

    ui->textBrowser_console->append("Program loaded! :3");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_enableArgs()
{
    LocoPacket _packet(ui->lineEdit_opcode->text());
    int _args = _packet.get_packetLen() - 2;
    if (_args > 0) {
        ui->lineEdit_arg1->setEnabled(true);
    } else {
        ui->lineEdit_arg1->setEnabled(false);
        ui->lineEdit_arg2->setEnabled(false);
        return;
    }
    if (_args > 1) {
        ui->lineEdit_arg2->setEnabled(true);
    } else {
        ui->lineEdit_arg2->setEnabled(false);
    }
}

void MainWindow::do_genPacket()
{
    QString _hex = "";
    outgoingPacket.set_allFromHex(ui->lineEdit_opcode->text());
    int _numArgs = outgoingPacket.get_packetLen() - 2;
    LocoPacket * _packet;
    _hex.append(ui->lineEdit_opcode->text());
    if (_numArgs > 0) {
        _hex.append(ui->lineEdit_arg1->text());
    } if (_numArgs > 1) {
        _hex.append(ui->lineEdit_arg2->text());
    }
    _packet = new LocoPacket(_hex);
    _packet->do_genChecksum();

    outgoingPacket = *_packet;
    ui->textBrowser_packets->append(_packet->get_packet());
    ui->lineEdit_packet->setText(_packet->get_packet());
    QString _historyPacket = outgoingPacket.get_packet();
    int _index = ui->comboBox_packetHistory->findText(_historyPacket);
    if (_index == -1) {
        ui->comboBox_packetHistory->addItem(_historyPacket);
    }
}

void MainWindow::do_initStaticOP(LocoPacket _locopacket)
{
    _locopacket.do_addStaticOP("85", "Global IDLE", "Put track into IDLE mode.");
    _locopacket.do_addStaticOP("83", "Global ON", "Put track into ON mode.");
    _locopacket.do_addStaticOP("82", "Global OFF", "Put track into OFF mode.");
    _locopacket.do_addStaticOP("BF", "OPC_LOCO_ADR", "Request loco address");
    _locopacket.do_addStaticOP("BD", "OPC_SW_ACK", "Request switch with acknowledge function");
    _locopacket.do_addStaticOP("BC", "OPC_SW_STATE", "Request state of switch");
    _locopacket.do_addStaticOP("BB", "OPC_RQ_SL_DATA", "Request SLOT DATA / status block");
    _locopacket.do_addStaticOP("BA", "OPC_MOVE_SLOTS", "MOVE slot SRC to DST");
    _locopacket.do_addStaticOP("B9", "OPC_LINK_SLOTS", "LINK slot ARG1 to slot ARG2");
    _locopacket.do_addStaticOP("B8", "OPC_UNLINK_SLOTS", "UNLINK slot ARG1 from slot ARG2");
    _locopacket.do_addStaticOP("B6", "OPC_CONSIST_FUNT", "SET FUNC bits in a consist uplink element");
    _locopacket.do_addStaticOP("B5", "OPC_SLOT_STAT1", "Write slot stat1");
    _locopacket.do_addStaticOP("B4", "OPC_LONG_ACK", "Long Acknowledge");
    _locopacket.do_addStaticOP("B2", "OPC_INPUT_REP", "General sensor input codes");
    _locopacket.do_addStaticOP("B1", "OPC_SW_REP", "Turnout sensor state report");
    _locopacket.do_addStaticOP("B0", "OPC_SW_REQ", "Request switch function.");
    _locopacket.do_addStaticOP("A2", "OPC_LOCO_SND", "Set SLOT sound functions.");
    _locopacket.do_addStaticOP("A1", "OPC_LOCO_DIRF", "Set SLOT direction, F0-4 state.");
    _locopacket.do_addStaticOP("A0", "OPC_LOCO_SPD", "Set SLOT speed");
    _locopacket.do_addStaticOP("EF", "OPC_WR_SL_DATA", "Write SLOT data (10 bytes)");
    _locopacket.do_addStaticOP("E7", "OPC_SL_RD_DATA", "Read SLOT data");
    _locopacket.do_addStaticOP("E5", "OPC_PEER_XFER", "Move 8 bytes peer to peer src->dst");
    _locopacket.do_addStaticOP("ED", "OPC_IMM_PACKET", "Send n-byte packet immediate");

    //_locopacket.do_addStaticOP("", "", "");
}

void MainWindow::do_loadOPComboBox()
{
    for (int _index = 0; _index < outgoingPacket.get_staticOPsize(); ++_index)
    {
        QString _text = outgoingPacket.get_staticOPname(_index);
        _text.append(" [" + outgoingPacket.get_staticOPhex(_index) + "]");
        ui->comboBox_opcodes->insertItem(_index, _text);
    }
}

void MainWindow::do_OPfromComboBox()
{
    QString _hex = outgoingPacket.get_staticOPhex(ui->comboBox_opcodes->currentIndex());
    ui->lineEdit_opcode->setText(_hex);
    do_enableArgs();
}

void MainWindow::do_serialRefreshList()
{
    QList<QSerialPortInfo> _ports = usbPorts.availablePorts();
    int _index = _ports.count();
    ui->comboBox_serialList->clear();
    for (int i = 0; i < _index; ++i)
    {
        ui->comboBox_serialList->insertItem(i, _ports.at(i).portName());
        ui->textBrowser_console->append(_ports.at(i).portName());
    }
}

void MainWindow::do_serialConnect()
{
    int _portIndex = ui->comboBox_serialList->currentIndex();
    QSerialPortInfo _device = usbPorts.availablePorts().at(_portIndex);
    loconet.do_serialOpen(_device);

    if (loconet.get_serialOpen())
    {
        ui->textBrowser_console->append("Serial port open :D");
        ui->pushButton_serialConnect->setEnabled(false);
        ui->pushButton_serialDisconnect->setEnabled(true);
        ui->comboBox_serialList->setEnabled(false);
        ui->pushButton_serialRefreshList->setEnabled(false);
    } else {
        ui->textBrowser_console->append("Serial port not open xC");
    }
}

void MainWindow::do_serialDisconnect()
{
    if (loconet.get_serialOpen()) {
        loconet.do_serialClose();
    }
    ui->textBrowser_console->append("Serial port closed :D");
    ui->pushButton_serialConnect->setEnabled(true);
    ui->pushButton_serialDisconnect->setEnabled(false);
    ui->comboBox_serialList->setEnabled(true);
    ui->pushButton_serialRefreshList->setEnabled(true);
}

void MainWindow::loadFromPacketHistory(int _index)
{
    QString _packet = ui->comboBox_packetHistory->itemText(_index);
    ui->lineEdit_packet->clear();
    ui->lineEdit_packet->setText(_packet);
}

void MainWindow::sendSerial()
{
    outgoingPacket.set_allFromHex(ui->lineEdit_packet->text());
    if (!outgoingPacket.is_validChk())
    {
        qDebug() << "Packet isn't right `_`";
        return;
    }

    ui->textBrowser_packets->append(outgoingPacket.get_packet().toLatin1());

    loconet.do_serialWrite(outgoingPacket);

    dumpQByteArray(outgoingPacket.get_QByteArray());
    qDebug() << "Firing off to serial: " << outgoingPacket.get_packet().toLatin1();
    qDebug() << outgoingPacket.get_QByteArray() << outgoingPacket.get_QBitArray();
}

void MainWindow::displayPacket(LocoPacket _packet)
{
    qDebug() << "Reading packet to text browser.";
    //ui->textBrowser_console->append(QTime::currentTime().toString("HH:mm:ss:zzz ") + _packet.get_packet());
    // Sort packets for easier reading
    QString _op = _packet.get_OPcode();
    if (_op == "B2")
    {
        ui->textBrowser_sorted_b2->append(_packet.get_packet());
    } else if (_op == "A0") {
        ui->textBrowser_sorted_a0->append(_packet.get_packet());
    } else if (_op == "E7") {
        ui->textBrowser_sorted_e7->append(_packet.get_packet());
    }
}

void MainWindow::dumpQByteArray(QByteArray _packet)
{
    LocoPacket _localPacket = LocoPacket(_packet.toHex());
    qDebug() << _packet.toHex();
    ui->textBrowser_console->append(incomingPacket.get_packet());
    qDebug() << _localPacket.get_packet();
}

void MainWindow::do_packetTimer()
{
    sendSerial();
}

void MainWindow::do_timerToggle()
{
    if (packetTimer->isActive())
    {
        packetTimer->stop();
        ui->pushButton_timerToggle->setText("Start Timer");
        return;
    }
    int _period = ui->spinBox_timerPeriod->value();
    packetTimer->start(_period*1000);
    ui->pushButton_timerToggle->setText("Stop Timer");
}

void MainWindow::printDescriptions(QString description)
{
    ui->textBrowser_console->append(QTime::currentTime().toString("HH:mm:ss:zzz ") + description);
}

void MainWindow::updateTrains (LocoTrain _train)
{
    QVector<LocoTrain> _trainList = loconet.get_trains();
    ui->textBrowser_trains->clear();
    for (int _index = 0; _index < _trainList.count(); ++_index)
    {
        ui->textBrowser_trains->append(_trainList[_index].get_descrtiption());
    }
    if (db.open()) {
        QString _adr = _train.get_adr().get_hex();
        QString _slot = _train.get_slot().get_hex();
        QString _speed = _train.get_speed().get_hex();
        QString _dir = QString::number(_train.get_direction());
        QString _query = "SELECT * FROM public.track_trains;";// WHERE slot='";
        //_query += _slot;
        //_query += "';";
        runQuery(_query);
        return;
        ui->textBrowser_sql->append(_query);
        if (dbQuery.exec(_query)) {
            ui->textBrowser_sql->append(QString::number(dbQuery.size()) + ", " + _query);
            if (dbQuery.size() == 0) {
                _query = "INSERT INTO public.track_trains (adr, slot, speed, dir)"
                        "VALUES (:adr, :slot, :speed, :dir);";
                dbQuery.exec(_query);
                ui->textBrowser_sql->append(dbQuery.lastError().text());
            }
        } else {
            ui->textBrowser_console->append(dbQuery.lastError().text());
        }
    }
}

void MainWindow::connectDB()
{
    // Collect information from window
    QString hostname = ui->lineEdit_hostname->text();
    int port = ui->spinBox_port->value();
    QString database = ui->lineEdit_database->text();
    QString username = ui->lineEdit_user->text();
    QString password = ui->lineEdit_password->text();

    // Load connection details
    db.setHostName(hostname);
    db.setPort(port);
    db.setUserName(username);
    db.setPassword(password);
    db.setDatabaseName(database);

    // Attempt to open database
    if (!db.open())
    {
        ui->textBrowser_sql->append("Opening postgresql database failed D:");
        ui->textBrowser_sql->append(db.lastError().text());
    } else {
        ui->textBrowser_sql->append("Database opened. Connection test appears successful :)");
        ui->textBrowser_sql->append(db.tables().join(",\n")); // Get a list of tables in the database and display them.
        ui->pushButton_connect->setEnabled(false);
        ui->pushButton_disconnect->setEnabled(true);
        ui->pushButton_tableText->setEnabled(true);
        ui->pushButton_queryModel->setEnabled(true);
        ui->pushButton_runQuery->setEnabled(true);
    }

}

void MainWindow::disconnectDB()
{
    if (!db.isOpen())
    {
        ui->textBrowser_sql->append("Database appears to already be closed? Wat do?!");
        return;
    } else {
        ui->textBrowser_sql->append("Trying to close the database.");
        db.close();
        if (!db.isOpen())
        {
            ui->textBrowser_sql->append("Database closed.");
            ui->pushButton_connect->setEnabled(true);
            ui->pushButton_disconnect->setEnabled(false);
            ui->pushButton_tableText->setEnabled(false);
            ui->pushButton_queryModel->setEnabled(false);
            ui->pushButton_runQuery->setEnabled(false);
        }
    }
}

/*
void MainWindow::tableText()
{
    if (!db.isOpen())
    {
        ui->textBrowser_output->append("The database isn't open.. :(");
    }

    ui->textBrowser_output->append("viewTable(): dumping table to this text box.");

    //INSERT INTO public.testsql VALUES('hi', 1);
    if (!dbQuery.exec("SELECT * FROM public.testsql"))
    {
        ui->textBrowser_output->append("Query failed :(");
        ui->textBrowser_output->append(dbQuery.lastError().text());
        return;
    }
    while (dbQuery.next())
    {
        for (int i = 0; i < 2; i++)
        {
            ui->textBrowser_output->append(dbQuery.value(i).toString());
        }
    }
    ui->textBrowser_output->append("viewTable(): done.");
}
*/

/*
void MainWindow::queryModel()
{
    ui->textBrowser_output->append("queryModel(): opening query view.");

    // Build the query from text entered in MainWindow.
    // Match the form "SELECT * FROM schema.table" to agree with Postgresql.
    schema = ui->lineEdit_schema->text();
    table = ui->lineEdit_table->text();
    QString tmpQuery = "SELECT * FROM ";
    tmpQuery.append(schema);
    tmpQuery.append(".");
    tmpQuery.append(table);

    // Load our query text into a query model
    dbQueryModel->setQuery(tmpQuery, db);

    // Check if the query model is correct
    if (dbQueryModel->lastError().isValid())
    {
        ui->textBrowser_output->append(dbQueryModel->lastError().text());
        ui->textBrowser_output->append("Issue with dbQueryModel listed above, exiting queryModel().");
        return;
    }

    // Load the query model into the table view. This is an attempt at View/Model practices.
    dbTableView->setModel(dbQueryModel);
    dbTableView->show();
}
*/

void MainWindow::manualQuery() {
    runQuery("");
}

void MainWindow::runQuery(QString _query)
{
    QString queryString = _query; /* Pull fresh query from the main interface */
    if (queryString == "") {
        queryString = ui->lineEdit_query->text();
    }
    if (!db.isOpen()) /* Check that the database is still open */
    {
        ui->textBrowser_sql->append("Database doesn't appear to be open :C");
        return;
    } else if (dbQuery.exec(queryString)) /* Execute the query and check that it succeeds */
    {
        ui->textBrowser_sql->append("Query successful? :3");
        if (dbQuery.isSelect()) /* A SELECT query was run */
        {
            ui->textBrowser_sql->append("Detected that the query is type SELECT.");
            while (dbQuery.next()) /* Must prime the results by calling .next(); the first row is always null */
            {
                int queryValue = 0;
                QString queryResults = "";
                while (!dbQuery.isNull(queryValue)) /* Cycle through the columns */
                {
                    queryResults.append("[");
                    queryResults.append(QString("%1").arg(queryValue)); /* Cast the int as QString */
                    queryResults.append("]: ");
                    queryResults.append(dbQuery.value(queryValue).toString());
                    queryValue++; /* Go to next column in row */
                }
                ui->textBrowser_sql->append(queryResults);
            }
        } else { /* Query was not type SELECT */
            ui->textBrowser_sql->append("Detected that the query is NOT type SELECT.");
            ui->textBrowser_sql->append("Number of rows affected: " + QString("%1").arg(dbQuery.numRowsAffected()));
        }
    } else { /* The query was not successful */
        ui->textBrowser_sql->append("Dun goofed. This shouldn't happen.");
        ui->textBrowser_sql->append(dbQuery.lastError().text());
    }
}

void MainWindow::do_resetTrack() {
    if (ui->lineEdit_packet->text() == "827D")
    {
        ui->lineEdit_opcode->setText("83");
        do_genPacket();
        sendSerial();
        ui->pushButton_resetTrack->setText("Enable Track.");
    } else {
        ui->lineEdit_opcode->setText("82");
        do_genPacket();
        sendSerial();
        ui->pushButton_resetTrack->setText("Disable Track.");
    }
}

/* Flippity Bit
 * That code was shi<malloc_error>
 */







