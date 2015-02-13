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

    ui->lineEdit_chk->setEnabled(false);

    usbBuffer = new QSerialPort;

    connect(ui->pushButton_genPacket, SIGNAL(clicked()), this, SLOT(do_genPacket()));
    connect(ui->lineEdit_opcode, SIGNAL(returnPressed()), this, SLOT(do_enableArgs()));
    connect(ui->comboBox_opcodes, SIGNAL(currentIndexChanged(int)), this, SLOT(do_OPfromComboBox()));
    connect(ui->pushButton_serialRefreshList, SIGNAL(clicked()), this, SLOT(do_serialRefreshList()));
    connect(ui->pushButton_serialConnect, SIGNAL(clicked()), this, SLOT(do_serialConnect()));
    connect(ui->pushButton_serialDisconnect, SIGNAL(clicked()), this, SLOT(do_serialDisconnect()));
    connect(usbBuffer, SIGNAL(readyRead()), this, SLOT(readSerial()));
    connect(ui->pushButton_sendPacket, SIGNAL(clicked()), this, SLOT(sendSerial()));

    ui->comboBox_opcodes->setEditable(false);
    ui->comboBox_opcodes->setInsertPolicy(QComboBox::InsertAtBottom);

    do_loadOPComboBox();
    do_serialRefreshList();

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

    if (ui->lineEdit_chk->text() == "")
    {
        _packet = new LocoPacket(_hex);
        _packet->do_genChecksum();
        //ui->lineEdit_chk->setText(_packet->getPacket());
    } else {
        _hex.append(ui->lineEdit_chk->text());
        _packet = new LocoPacket(_hex);
    }

    outgoingPacket = *_packet;
    ui->textBrowser_packets->append(_packet->get_packet());
    ui->lineEdit_packet->setText(_packet->get_packet());
    ui->comboBox_packetHistory->addItem(_packet->get_packet());
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
    usbBuffer->setPort(usbPorts.availablePorts().at(_portIndex));
    usbBuffer->setBaudRate(57600);
    usbBuffer->open(QIODevice::ReadWrite);
    if (usbBuffer->isOpen())
    {
        ui->textBrowser_console->append("Serial port open :D");
        ui->pushButton_serialConnect->setEnabled(false);
        ui->pushButton_serialDisconnect->setEnabled(true);
        ui->comboBox_serialList->setEnabled(false);
        ui->pushButton_serialRefreshList->setEnabled(false);
    } else {
        ui->textBrowser_console->append("Serial port not open xC [" + usbBuffer->errorString() + "]");
    }
}

void MainWindow::do_serialDisconnect()
{
    if (usbBuffer->isOpen()) {
        usbBuffer->close();
    }
    ui->textBrowser_console->append("Serial port closed :D");
    ui->pushButton_serialConnect->setEnabled(true);
    ui->pushButton_serialDisconnect->setEnabled(false);
    ui->comboBox_serialList->setEnabled(true);
    ui->pushButton_serialRefreshList->setEnabled(true);
}

void MainWindow::sendSerial()
{
    if (!outgoingPacket.is_validChk())
    {
        qDebug() << "Packet isn't right `_`";
        return;
    }
    ui->textBrowser_packets->append(outgoingPacket.get_packet().toLatin1());
    if (!usbBuffer->isOpen())
    {
        qDebug() << "Serial isn't open...";
        return;
    }
    usbBuffer->write(outgoingPacket.get_packet().toLatin1());
}

void MainWindow::readSerial()
{
    if (!usbBuffer->isOpen())
    {
        qDebug() << "Serial port suddenly isn't open x.x";
        do_serialDisconnect();
        return;
    }
    QByteArray _data;
    while(usbBuffer->waitForReadyRead(50))
    {
        qDebug() << "Reading serial ^_^";
        _data = usbBuffer->read(1);

        if (incomingPacket.is_validChk()) {
            ui->textBrowser_console->append(incomingPacket.get_packet());
            incomingPacket.set_allFromHex(_data.toHex());
        }
        incomingPacket.do_appendByte(_data.toHex());
    }
}

/* Flippity Bit
 * That code was shi<malloc_error>
 */






