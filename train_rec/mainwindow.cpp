#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
 * train_rec - "Train Receiver"
 * Control a Digitrax track from the internet
 * 
 * Christopher Bero [csb0019@uah.edu]
 * Team 4A
 *
 * I've tried hard not to step on any copyrights associated with Digitrax code,
 * don't sue me please.
 */

/**
 * Conventions:
 * Underscores indicate a local variable to a function
 * set_ to change a member variable
 * get_ to retrieve a member variable or variant
 * do_ to complete a task or slot
 * is_ to query a state of the object
 * handle_ to take care of a signal, similar to do_
 * do_run() in each thread class allows for proper instantiation of child variables
 */

/**
 * Program Structure:
 * Four threads: GUI, UDP, SQL, and Serial/USB
 * All interaction should be via Signals/Slots through the GUI thread.
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
    connect(locoudp, &LocoUDP::incomingRequest, this, &MainWindow::do_packetReceived);
    connect(&threadUDP, &QThread::finished, locoudp, &QObject::deleteLater);
    // Handle Initializing from Sig/Slot
    connect(ui->pushButton_thread_beginUDP, SIGNAL(clicked()), locoudp, SLOT(do_run()));
    connect(ui->pushButton_thread_beginUDP, SIGNAL(clicked(bool)), ui->pushButton_thread_beginUDP, SLOT(setEnabled(bool)));

    // Serial
    connect(this, &MainWindow::locoserial_open, locoserial, &LocoSerial::do_open);
    connect(this, &MainWindow::locoserial_write, locoserial, &LocoSerial::do_writePacket);
    connect(locoserial, &LocoSerial::receivedPacket, this, &MainWindow::do_packetReceived); // QT-5 style works
    connect(locoserial, &LocoSerial::writtenBytes, this, &MainWindow::do_bytesWritten);
    connect(locoserial, &LocoSerial::serialOpened, this, &MainWindow::handle_serialOpened);
    connect(locoserial, &LocoSerial::serialClosed, this, &MainWindow::handle_serialClosed);
    connect(locoserial, &LocoSerial::blockUpdated, locosql, &LocoSQL::do_updateBlock);
    connect(locoserial, &LocoSerial::trainUpdated, locosql, &LocoSQL::do_updateTrain);
    connect(&threadSerial, &QThread::finished, locoserial, &QObject::deleteLater);
    // Handle Initializing from Sig/Slot
    connect(ui->pushButton_thread_beginSerial, SIGNAL(clicked()), locoserial, SLOT(do_run()));
    connect(ui->pushButton_thread_beginSerial, SIGNAL(clicked(bool)), ui->pushButton_thread_beginSerial, SLOT(setEnabled(bool)));

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
    // Handle Initializing from Sig/Slot
    connect(ui->pushButton_thread_beginSQL, SIGNAL(clicked()), locosql, SLOT(do_run()));
    connect(ui->pushButton_thread_beginSQL, SIGNAL(clicked(bool)), ui->pushButton_thread_beginSQL, SLOT(setEnabled(bool)));

    // Kickstart threads
    threadSerial.start();
    threadSQL.start();
    threadUDP.start();

    // Configure interface
    ui->comboBox_opcodes->setEditable(false);
    ui->comboBox_opcodes->setInsertPolicy(QComboBox::InsertAtBottom);
    do_loadOPComboBox();
    do_refreshSerialList();

    if (debug) qDebug() << timeStamp() << "Interface loaded.";
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

QString MainWindow::timeStamp()
{
    return(QTime::currentTime().toString("[HH:mm:ss:zzz] "));
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
    ui->textBrowser_packets->append(timeStamp() + "Generated: " + _packet->get_packet());
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
        qDebug() << timeStamp() << "database opcodes and names don't match.";
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

void MainWindow::do_packetReceived(LocoPacket _packet)
{
    if (debug) qDebug() << timeStamp() << "Reading packet to text browser. " << _packet.get_packet();
    ui->textBrowser_console->append(timeStamp() + _packet.get_packet());
    // Sort packets for easier reading
    QString _op = _packet.get_OPcode();
    if (_op == "B2")
    {
        ui->textBrowser_sorted_b2->append(timeStamp() + _packet.get_packet());
    } else if (_op == "A0") {
        ui->textBrowser_sorted_a0->append(timeStamp() + _packet.get_packet());
    } else if (_op == "E7") {
        ui->textBrowser_sorted_e7->append(timeStamp() + _packet.get_packet());
    }
}

void MainWindow::do_bytesWritten(QByteArray _bytes)
{
    QString _byteText = QString::fromLatin1(_bytes.toHex());
    if (debug) qDebug() << timeStamp() << "Writing bytes to text browser. " << _byteText;
    ui->textBrowser_packets->append(timeStamp() + "Written: " +  _byteText);
}

void MainWindow::do_printDescriptions(QString description)
{
    ui->textBrowser_console->append(timeStamp() + description);
}

/*
 * LOCOSERIAL METHODS
 */

void MainWindow::handle_serialOpened()
{
    ui->textBrowser_console->append(timeStamp() + "Serial port opened x)");
    ui->pushButton_serialConnect->setEnabled(false);
    ui->pushButton_serialDisconnect->setEnabled(true);
    ui->comboBox_serialList->setEnabled(false);
    ui->pushButton_serialRefreshList->setEnabled(false);
}

void MainWindow::handle_serialClosed()
{
    ui->textBrowser_console->append(timeStamp() + "Serial port closed :D");
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
        qDebug() << timeStamp() << "Packet isn't right ~_~;";
        return;
    }
    ui->textBrowser_packets->append(timeStamp() + "Asking for write: " + outgoingPacket.get_packet().toLatin1());

    // Only interact with a thread via Sig/Slots.
    emit locoserial_write(outgoingPacket);

    if (debug) qDebug() << timeStamp() << "Firing off to serial: " << outgoingPacket.get_packet().toLatin1();
    if (debug) qDebug() << timeStamp() << outgoingPacket.get_QByteArray() << outgoingPacket.get_QBitArray();
}

/*
 * SQL METHODS
 */

void MainWindow::handle_DBopened()
{
    ui->textBrowser_sql->append(timeStamp() + "Database opened. Connection appears successful :)");
    ui->pushButton_connect->setEnabled(false);
    ui->lineEdit_database->setEnabled(false);
    ui->lineEdit_hostname->setEnabled(false);
    ui->lineEdit_password->setEnabled(false);
    ui->lineEdit_user->setEnabled(false);
    ui->spinBox_port->setEnabled(false);
    ui->pushButton_disconnect->setEnabled(true);
}

void MainWindow::handle_DBclosed()
{
    ui->textBrowser_sql->append(timeStamp() + "Database closed.");
    ui->pushButton_connect->setEnabled(true);
    ui->lineEdit_database->setEnabled(true);
    ui->lineEdit_hostname->setEnabled(true);
    ui->lineEdit_password->setEnabled(true);
    ui->lineEdit_user->setEnabled(true);
    ui->spinBox_port->setEnabled(true);
    ui->pushButton_disconnect->setEnabled(false);
}

void MainWindow::do_connectDB()
{
    // Collect information from window
    QString hostname = ui->lineEdit_hostname->text();
    int port = ui->spinBox_port->value();
    QString database = ui->lineEdit_database->text();
    QString username = ui->lineEdit_user->text();
    QString password = ui->lineEdit_password->text();

    emit locosql_open(hostname, port, database, username, password);
}







