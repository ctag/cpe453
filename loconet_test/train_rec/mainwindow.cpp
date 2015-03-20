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

bool MainWindow::debug = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    locoserial.moveToThread(&serialThread);
    serialThread.start();

    locosql.moveToThread(&sqlThread);
    sqlThread.start();

    outgoingPacket.clear();

    ui->lineEdit_opcode->setInputMask("hh");
    ui->lineEdit_arg1->setInputMask("hh");
    ui->lineEdit_arg2->setInputMask("hh");

    connect(ui->pushButton_genPacket, SIGNAL(clicked()), this, SLOT(do_genPacket()));
    connect(ui->lineEdit_opcode, SIGNAL(editingFinished()), this, SLOT(do_enableArgs()));
    connect(ui->pushButton_serialRefreshList, SIGNAL(clicked()), this, SLOT(do_refreshSerialList()));
    connect(ui->pushButton_serialConnect, SIGNAL(clicked()), this, SLOT(do_openSerial()));
    connect(ui->pushButton_serialDisconnect, SIGNAL(clicked()), this, SLOT(do_closeSerial()));
    connect(ui->pushButton_sendPacket, SIGNAL(clicked()), this, SLOT(do_sendSerial()));
    connect(&locoserial, &LocoSerial::receivedPacket, this, &MainWindow::do_displayPacket); // QT-5 style works
    //connect(&locoserial, &LocoSerial::, this, &MainWindow::do_printDescriptions);
    connect(ui->pushButton_connect, SIGNAL(clicked()), this, SLOT(do_connectDB()));
    connect(ui->pushButton_disconnect, SIGNAL(clicked()), this, SLOT(do_disconnectDB()));
    connect(&locosql, &LocoSQL::DBopened, this, &MainWindow::handle_DBopened);
    connect(&locosql, &LocoSQL::DBclosed, this, &MainWindow::handle_DBclosed);
    connect(&locoserial, &LocoSerial::serialOpened, this, &MainWindow::handle_serialOpened);
    connect(&locoserial, &LocoSerial::serialClosed, this, &MainWindow::handle_serialClosed);
    connect(&locoserial, &LocoSerial::blockUpdated, &locosql, &LocoSQL::do_updateBlock);

    ui->comboBox_opcodes->setEditable(false);
    ui->comboBox_opcodes->setInsertPolicy(QComboBox::InsertAtBottom);

    //do_loadOPComboBox();
    do_refreshSerialList();

    if (debug) qDebug() << "Interface loaded.";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_enableArgs()
{
    LocoPacket _packet(ui->lineEdit_opcode->text());
    int _args = _packet.get_finalSize() - 2;
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
    int _numArgs = outgoingPacket.get_finalSize() - 2;
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
}

/*
void MainWindow::do_initStaticOP()
{
    loconet.do_addStaticOP("85", "Global IDLE", "Put track into IDLE mode.");
    loconet.do_addStaticOP("83", "Global ON", "Put track into ON mode.");
    loconet.do_addStaticOP("82", "Global OFF", "Put track into OFF mode.");
    loconet.do_addStaticOP("BF", "OPC_LOCO_ADR", "Request loco address");
    loconet.do_addStaticOP("BD", "OPC_SW_ACK", "Request switch with acknowledge function");
    loconet.do_addStaticOP("BC", "OPC_SW_STATE", "Request state of switch");
    loconet.do_addStaticOP("BB", "OPC_RQ_SL_DATA", "Request SLOT DATA / status block");
    loconet.do_addStaticOP("BA", "OPC_MOVE_SLOTS", "MOVE slot SRC to DST");
    loconet.do_addStaticOP("B9", "OPC_LINK_SLOTS", "LINK slot ARG1 to slot ARG2");
    loconet.do_addStaticOP("B8", "OPC_UNLINK_SLOTS", "UNLINK slot ARG1 from slot ARG2");
    loconet.do_addStaticOP("B6", "OPC_CONSIST_FUNT", "SET FUNC bits in a consist uplink element");
    loconet.do_addStaticOP("B5", "OPC_SLOT_STAT1", "Write slot stat1");
    loconet.do_addStaticOP("B4", "OPC_LONG_ACK", "Long Acknowledge");
    loconet.do_addStaticOP("B2", "OPC_INPUT_REP", "General sensor input codes");
    loconet.do_addStaticOP("B1", "OPC_SW_REP", "Turnout sensor state report");
    loconet.do_addStaticOP("B0", "OPC_SW_REQ", "Request switch function.");
    loconet.do_addStaticOP("A2", "OPC_LOCO_SND", "Set SLOT sound functions.");
    loconet.do_addStaticOP("A1", "OPC_LOCO_DIRF", "Set SLOT direction, F0-4 state.");
    loconet.do_addStaticOP("A0", "OPC_LOCO_SPD", "Set SLOT speed");
    loconet.do_addStaticOP("EF", "OPC_WR_SL_DATA", "Write SLOT data (10 bytes)");
    loconet.do_addStaticOP("E7", "OPC_SL_RD_DATA", "Read SLOT data");
    loconet.do_addStaticOP("E5", "OPC_PEER_XFER", "Move 8 bytes peer to peer src->dst");
    loconet.do_addStaticOP("ED", "OPC_IMM_PACKET", "Send n-byte packet immediate");

    //loconet.do_addStaticOP("", "", "");
}
*/

void MainWindow::do_loadOPComboBox()
{
    ui->comboBox_opcodes->clear();
    LocoPacket _tmp;
    QVector<QString> _opcodes = _tmp.get_DBopcodes();
    QVector<QString> _names = _tmp.get_DBnames();
    if (_opcodes.count() != _names.count())
    {
        qDebug() << "database opcodes and names don't match.";
        return;
    }
    for (int _index = 0; _index < _opcodes.count(); ++_index)
    {
        QString _text = _names[_index];
        _text.append(" [" + _opcodes[_index] + "]");
        ui->comboBox_opcodes->insertItem(_index, _text);
    }
}

void MainWindow::do_OPfromComboBox()
{
    LocoPacket _tmp;
    QVector<QString> _opcodes = _tmp.get_DBopcodes();
    QString _hex = _opcodes[ui->comboBox_opcodes->currentIndex()];
    ui->lineEdit_opcode->setText(_hex);
    do_enableArgs();
}

void MainWindow::do_refreshSerialList()
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

void MainWindow::handle_serialOpened()
{
    ui->pushButton_serialConnect->setEnabled(false);
    ui->pushButton_serialDisconnect->setEnabled(true);
    ui->comboBox_serialList->setEnabled(false);
    ui->pushButton_serialRefreshList->setEnabled(false);
}

void MainWindow::do_openSerial()
{
    int _portIndex = ui->comboBox_serialList->currentIndex();
    QSerialPortInfo _device = usbPorts.availablePorts().at(_portIndex);
    locoserial.open(_device);
}

void MainWindow::handle_serialClosed()
{
    ui->textBrowser_console->append("Serial port closed :D");
    ui->pushButton_serialConnect->setEnabled(true);
    ui->pushButton_serialDisconnect->setEnabled(false);
    ui->comboBox_serialList->setEnabled(true);
    ui->pushButton_serialRefreshList->setEnabled(true);
}

void MainWindow::do_closeSerial()
{
    locoserial.close();
}

void MainWindow::do_sendSerial()
{
    outgoingPacket.set_allFromHex(ui->lineEdit_packet->text());
    if (!outgoingPacket.validChk())
    {
        if (debug) qDebug() << "Packet isn't right `_`";
        return;
    }

    ui->textBrowser_packets->append(outgoingPacket.get_packet().toLatin1());

    locoserial.write(outgoingPacket);

    do_dumpQByteArray(outgoingPacket.get_QByteArray());
    if (debug) qDebug() << "Firing off to serial: " << outgoingPacket.get_packet().toLatin1();
    if (debug) qDebug() << outgoingPacket.get_QByteArray() << outgoingPacket.get_QBitArray();
}

void MainWindow::do_displayPacket(LocoPacket _packet)
{
    if (debug) qDebug() << "Reading packet to text browser. " << _packet.get_packet();
    ui->textBrowser_console->append(QTime::currentTime().toString("HH:mm:ss:zzz ") + _packet.get_packet());
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

void MainWindow::do_dumpQByteArray(QByteArray _packet)
{
    LocoPacket _localPacket = LocoPacket(_packet.toHex());
    if (debug) qDebug() << _packet.toHex();
    ui->textBrowser_console->append(incomingPacket.get_packet());
    if (debug) qDebug() << _localPacket.get_packet();
}

void MainWindow::do_printDescriptions(QString description)
{
    ui->textBrowser_console->append(QTime::currentTime().toString("HH:mm:ss:zzz ") + description);
}

/*
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

void MainWindow::updateBlocks (LocoBlock _block)
{
    if (db.open()) {
        dbQuery.prepare("INSERT INTO cpe453.track_ds (ds_id, status) "
                        "VALUES (:id, :status) "
                        "ON DUPLICATE KEY "
                        "UPDATE ds_id=:id, status=:status;");
        QString _id = QString::fromLatin1(_block.get_adr());
        bool _status = _block.get_occupied();
        dbQuery.bindValue(":id", _id);
        dbQuery.bindValue(":status", _status);

        dbQuery.exec();
        ui->textBrowser_sql->append("ran block update query. id [" + _id + "] status [" + QString::number(_status) + "]");
    }
}
*/

void MainWindow::handle_DBopened()
{
    ui->textBrowser_sql->append("Database opened. Connection test appears successful :)");
    ui->pushButton_connect->setEnabled(false);
    ui->pushButton_disconnect->setEnabled(true);
    ui->pushButton_runQuery->setEnabled(true);
}

void MainWindow::handle_DBclosed()
{
    ui->textBrowser_sql->append("Database closed.");
    ui->pushButton_connect->setEnabled(true);
    ui->pushButton_disconnect->setEnabled(false);
    ui->pushButton_runQuery->setEnabled(false);
}

void MainWindow::do_connectDB()
{
    // Collect information from window
    QString hostname = ui->lineEdit_hostname->text();
    int port = ui->spinBox_port->value();
    QString database = ui->lineEdit_database->text();
    QString username = ui->lineEdit_user->text();
    QString password = ui->lineEdit_password->text();

    locosql.do_openDB(hostname, port, database, username, password);
}

void MainWindow::do_disconnectDB()
{
    locosql.do_closeDB();
}

/* Flippity Bit
 * That code was shi<malloc_error>
 */







