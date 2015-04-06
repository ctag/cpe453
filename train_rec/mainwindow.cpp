#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
 * Conventions:
 * Underscores indicate a local variable to a function
 * set_ to change a member variable
 * get_ to retrieve a member variable or variant
 * do_ to complete a task or slot
 * is_ to query a state of the object
 * handle_ to take care of a signal
 */

bool MainWindow::debug = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Declare threads
    locoserial = new LocoSerial;
    locoserial->moveToThread(&threadSerial);
    locosql = new LocoSQL;
    locosql->moveToThread(&threadSQL);
    locoudp = new LocoUDP;
    locoudp->moveToThread(&threadUDP);

    outgoingPacket.clear();

    ui->lineEdit_opcode->setInputMask("hh");
    ui->lineEdit_arg1->setInputMask("hh");
    ui->lineEdit_arg2->setInputMask("hh");

    // GUI
    connect(ui->comboBox_opcodes, SIGNAL(activated(int)), this, SLOT(do_OPfromComboBox()));
    connect(ui->pushButton_genPacket, SIGNAL(clicked()), this, SLOT(do_genPacket()));
    connect(ui->lineEdit_opcode, SIGNAL(editingFinished()), this, SLOT(do_enableArgs()));
    connect(ui->pushButton_serialRefreshList, SIGNAL(clicked()), this, SLOT(do_refreshSerialList()));
    connect(ui->pushButton_serialConnect, SIGNAL(clicked()), this, SLOT(do_openSerial()));
    connect(ui->pushButton_serialDisconnect, SIGNAL(clicked()), locoserial, SLOT(do_close()));
    connect(ui->pushButton_sendPacket, SIGNAL(clicked()), this, SLOT(do_sendSerial()));
    connect(ui->pushButton_connect, SIGNAL(clicked()), this, SLOT(do_connectDB()));
    connect(ui->pushButton_disconnect, SIGNAL(clicked()), locosql, SLOT(do_closeDB()));

    // UDP
    connect(locoudp, &LocoUDP::incomingRequest, locoserial, static_cast<void (LocoSerial::*)(LocoPacket)>(&LocoSerial::do_writePacket));
    connect(locoudp, &LocoUDP::incomingRequest, this, &MainWindow::do_displayPacket);
    connect(&threadUDP, &QThread::finished, locoudp, &QObject::deleteLater);

    // Serial
    connect(this, &MainWindow::locoserial_open, locoserial, &LocoSerial::do_open);
    connect(this, &MainWindow::locoserial_write, locoserial, &LocoSerial::do_writePacket);
    connect(locoserial, &LocoSerial::receivedPacket, this, &MainWindow::do_displayPacket); // QT-5 style works
    connect(locoserial, &LocoSerial::serialOpened, this, &MainWindow::handle_serialOpened);
    connect(locoserial, &LocoSerial::serialClosed, this, &MainWindow::handle_serialClosed);
    connect(locoserial, &LocoSerial::blockUpdated, locosql, &LocoSQL::do_updateBlock);
    connect(locoserial, &LocoSerial::trainUpdated, locosql, &LocoSQL::do_updateTrain);
    connect(&threadSerial, &QThread::finished, locoserial, &QObject::deleteLater);

    // Macros / locoSQL
    connect(locosql, &LocoSQL::incomingRequest, locoserial, static_cast<void (LocoSerial::*)(LocoPacket)>(&LocoSerial::do_writePacket));
    connect(this, &MainWindow::locosql_open, locosql, &LocoSQL::do_openDB);
    connect(locosql, &LocoSQL::DBopened, this, &MainWindow::handle_DBopened);
    connect(locosql, &LocoSQL::DBclosed, this, &MainWindow::handle_DBclosed);
    connect(locosql, &LocoSQL::slotScan, locoserial, &LocoSerial::do_slotScan);
    connect(locosql, &LocoSQL::slotDispatch, locoserial, &LocoSerial::do_slotDispatch);
    connect(locosql, &LocoSQL::slotReq, locoserial, &LocoSerial::do_slotReq);
    connect(locosql, &LocoSQL::slotUse, locoserial, &LocoSerial::do_slotUse);
    connect(locosql, &LocoSQL::slotClear, locoserial, &LocoSerial::do_slotClear);
    connect(locosql, &LocoSQL::trackReset, locoserial, &LocoSerial::do_trackReset);
    connect(locosql, &LocoSQL::trackOn, locoserial, &LocoSerial::do_trackOn);
    connect(locosql, &LocoSQL::trackOff, locoserial, &LocoSerial::do_trackOff);
    connect(&threadUDP, &QThread::finished, locoudp, &QObject::deleteLater);

    // Kickstart threads
    threadSerial.start();
    locoserial->do_run();
    threadSQL.start();
    locosql->do_run();
    threadUDP.start();
    locoudp->do_run();
    locoudp->do_socketOpen(7755);

    // Configure interface
    ui->comboBox_opcodes->setEditable(false);
    ui->comboBox_opcodes->setInsertPolicy(QComboBox::InsertAtBottom);
    do_loadOPComboBox();
    do_refreshSerialList();

    if (debug) qDebug() << "Interface loaded.";
}

MainWindow::~MainWindow()
{
    // Clean up sub-threads
    threadSerial.quit();
    threadSerial.wait();
    threadSQL.quit();
    threadSQL.wait();
    threadUDP.quit();
    threadUDP.wait();

    delete ui;
}

/*
 * MAINWINDOW METHODS
 */

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

void MainWindow::do_printDescriptions(QString description)
{
    ui->textBrowser_console->append(QTime::currentTime().toString("HH:mm:ss:zzz ") + description);
}

/*
 * LOCOSERIAL METHODS
 */

void MainWindow::handle_serialOpened()
{
    ui->pushButton_serialConnect->setEnabled(false);
    ui->pushButton_serialDisconnect->setEnabled(true);
    ui->comboBox_serialList->setEnabled(false);
    ui->pushButton_serialRefreshList->setEnabled(false);
}

void MainWindow::handle_serialClosed()
{
    ui->textBrowser_console->append("Serial port closed :D");
    ui->pushButton_serialConnect->setEnabled(true);
    ui->pushButton_serialDisconnect->setEnabled(false);
    ui->comboBox_serialList->setEnabled(true);
    ui->pushButton_serialRefreshList->setEnabled(true);
}

void MainWindow::do_openSerial()
{
    int _portIndex = ui->comboBox_serialList->currentIndex();
    QSerialPortInfo _device = usbPorts.availablePorts().at(_portIndex);
    emit locoserial_open(_device);
}

void MainWindow::do_sendSerial()
{
    outgoingPacket.set_allFromHex(ui->lineEdit_packet->text());
    if (!outgoingPacket.validChk())
    {
        qDebug() << "Packet isn't right `_`";
        return;
    }
    ui->textBrowser_packets->append(outgoingPacket.get_packet().toLatin1());

    // Only interact with a thread via Sig/Slots.
    emit locoserial_write(outgoingPacket);

    if (debug) qDebug() << "Firing off to serial: " << outgoingPacket.get_packet().toLatin1();
    if (debug) qDebug() << outgoingPacket.get_QByteArray() << outgoingPacket.get_QBitArray();
}

/*
 * SQL METHODS
 */

void MainWindow::handle_DBopened()
{
    ui->textBrowser_sql->append("Database opened. Connection appears successful :)");
    ui->pushButton_connect->setEnabled(false);
    ui->pushButton_disconnect->setEnabled(true);
    //ui->pushButton_runQuery->setEnabled(true);
}

void MainWindow::handle_DBclosed()
{
    ui->textBrowser_sql->append("Database closed.");
    ui->pushButton_connect->setEnabled(true);
    ui->pushButton_disconnect->setEnabled(false);
    //ui->pushButton_runQuery->setEnabled(false);
}

void MainWindow::do_connectDB()
{
    // Collect information from window
    QString hostname = ui->lineEdit_hostname->text();
    int port = ui->spinBox_port->value();
    QString database = ui->lineEdit_database->text();
    QString username = ui->lineEdit_user->text();
    QString password = ui->lineEdit_password->text();

    //locosql->do_openDB(hostname, port, database, username, password);
    emit locosql_open(hostname, port, database, username, password);
}

/**
 * Flippity Bit
 * That code was shi<malloc_error>
 */







