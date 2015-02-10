#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    locopacket.set_allFromHex("00");
    do_initStaticOP(locopacket);

    ui->lineEdit_opcode->setInputMask("hh");
    ui->lineEdit_arg1->setInputMask("hh");
    ui->lineEdit_arg2->setInputMask("hh");
    ui->lineEdit_opcode->setText("");

    ui->lineEdit_chk->setEnabled(false);

    loconet = new QSerialPort;

    connect(ui->pushButton_genPacket, SIGNAL(clicked()), this, SLOT(do_genPacket()));
    connect(ui->lineEdit_opcode, SIGNAL(returnPressed()), this, SLOT(do_enableArgs()));
    connect(ui->comboBox_opcodes, SIGNAL(currentIndexChanged(int)), this, SLOT(do_OPfromComboBox()));

    ui->comboBox_opcodes->setEditable(false);
    ui->comboBox_opcodes->setInsertPolicy(QComboBox::InsertAtBottom);

    do_loadOPComboBox();
    do_listPorts();

    ui->textBrowser_console->append("Program loaded! :3");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_enableArgs()
{
    LocoPacket _packet(ui->lineEdit_opcode->text());
    int _args = _packet.get_numArgs();
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
    locopacket.set_allFromHex(ui->lineEdit_opcode->text());
    int _numArgs = locopacket.get_numArgs();
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

    ui->textBrowser_packets->append(_packet->get_packet());
    ui->lineEdit_packet->setText(_packet->get_packet());
    ui->comboBox_packetHistory->addItem(_packet->get_packet());
}

//LocoHex("B5"), LocoHex("B4"), LocoHex("B2"), LocoHex("B1"), LocoHex("B0")};
//", "OPC_LONG_ACK", "OPC_INPUT_REP", "OPC_SQ_REP", "OPC_SW_REQ"};
//int LocoPacket::opcodes_args[16] = {0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};

void MainWindow::do_initStaticOP(LocoPacket _locopacket)
{
    _locopacket.do_addStaticOP("85", "Global IDLE", "", 0);
    _locopacket.do_addStaticOP("83", "Global ON", "", 0);
    _locopacket.do_addStaticOP("82", "Global OFF", "", 0);
    _locopacket.do_addStaticOP("BF", "OPC_LOCO_ADR", "", 2);
    _locopacket.do_addStaticOP("BD", "OPC_SW_ACK", "", 2);
    _locopacket.do_addStaticOP("BC", "OPC_SW_STATE", "", 2);
    _locopacket.do_addStaticOP("BB", "OPC_RQ_SL_DATA", "", 2);
    _locopacket.do_addStaticOP("BA", "OPC_MOVE_SLOTS", "", 2);
    _locopacket.do_addStaticOP("B9", "OPC_LINK_SLOTS", "", 2);
    _locopacket.do_addStaticOP("B8", "OPC_UNLINK_SLOTS", "", 2);
    _locopacket.do_addStaticOP("B6", "OPC_CONSIST_FUNT", "", 2);
    _locopacket.do_addStaticOP("B5", "OPC_SLOT_STAT1", "", 2);
    _locopacket.do_addStaticOP("B4", "OPC_LONG_ACK", "", 2);
    _locopacket.do_addStaticOP("B2", "OPC_INPUT_REP", "", 2);
    _locopacket.do_addStaticOP("B1", "OPC_SW_REP", "", 2);
    _locopacket.do_addStaticOP("B0", "OPC_SW_REQ", "", 2);
}

void MainWindow::do_loadOPComboBox()
{
    for (int _index = 0; _index < locopacket.get_numStaticOP(); ++_index)
    {
        ui->comboBox_opcodes->insertItem(_index, locopacket.get_staticOPname(_index));
    }
}

void MainWindow::do_OPfromComboBox()
{
    QString _hex = locopacket.get_staticOPhex(ui->comboBox_opcodes->currentIndex());
    ui->lineEdit_opcode->setText(_hex);
    do_enableArgs();
}

void MainWindow::do_listPorts()
{
    QList<QSerialPortInfo> _ports = usbPorts.availablePorts();
    int _index = _ports.count();
    for (int i = 0; i < _index; ++i)
    {
        ui->textBrowser_console->append(_ports.at(i).portName());
    }
}

/* Flippity Bit
 * That code was stale as .
 */







