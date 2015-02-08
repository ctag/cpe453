#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    locopacket.define("00");
    initOPcodes(locopacket);



    ui->lineEdit_opcode->setInputMask("hh");
    ui->lineEdit_arg1->setInputMask("hh");
    ui->lineEdit_arg2->setInputMask("hh");
    ui->lineEdit_opcode->setText("");

    ui->lineEdit_chk->setEnabled(false);

    loconet = new QSerialPort;

    connect(ui->pushButton_genPacket, SIGNAL(clicked()), this, SLOT(genPacket()));
    connect(ui->lineEdit_opcode, SIGNAL(returnPressed()), this, SLOT(enableArgs()));
    connect(ui->comboBox_opcodes, SIGNAL(currentIndexChanged(int)), this, SLOT(updateOPcodeFromComboBox()));

    ui->comboBox_opcodes->setEditable(false);
    ui->comboBox_opcodes->setInsertPolicy(QComboBox::InsertAtBottom);

    loadOPcodeComboBox();
    listSerialPorts();

    ui->textBrowser_console->append("Program loaded! :3");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::enableArgs()
{
    LocoPacket _packet(ui->lineEdit_opcode->text());
    int _args = _packet.numArgs();
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

void MainWindow::genPacket()
{
    QString _hex = "";
    locopacket.define(ui->lineEdit_opcode->text());
    int _numArgs = locopacket.numArgs();
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
        _packet->genChecksum();
        //ui->lineEdit_chk->setText(_packet->getPacket());
    } else {
        _hex.append(ui->lineEdit_chk->text());
        _packet = new LocoPacket(_hex);
    }

    ui->textBrowser_packets->append(_packet->getPacket());
    ui->lineEdit_packet->setText(_packet->getPacket());
    ui->comboBox_packetHistory->addItem(_packet->getPacket());
}

//LocoHex("B5"), LocoHex("B4"), LocoHex("B2"), LocoHex("B1"), LocoHex("B0")};
//", "OPC_LONG_ACK", "OPC_INPUT_REP", "OPC_SQ_REP", "OPC_SW_REQ"};
//int LocoPacket::opcodes_args[16] = {0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};

void MainWindow::initOPcodes(LocoPacket _locopacket)
{
    _locopacket.addOPcode("85", "Global IDLE", "", 0);
    _locopacket.addOPcode("83", "Global ON", "", 0);
    _locopacket.addOPcode("82", "Global OFF", "", 0);
    _locopacket.addOPcode("BF", "OPC_LOCO_ADR", "", 2);
    _locopacket.addOPcode("BD", "OPC_SW_ACK", "", 2);
    _locopacket.addOPcode("BC", "OPC_SW_STATE", "", 2);
    _locopacket.addOPcode("BB", "OPC_RQ_SL_DATA", "", 2);
    _locopacket.addOPcode("BA", "OPC_MOVE_SLOTS", "", 2);
    _locopacket.addOPcode("B9", "OPC_LINK_SLOTS", "", 2);
    _locopacket.addOPcode("B8", "OPC_UNLINK_SLOTS", "", 2);
    _locopacket.addOPcode("B6", "OPC_CONSIST_FUNT", "", 2);
    _locopacket.addOPcode("B5", "OPC_SLOT_STAT1", "", 2);
    _locopacket.addOPcode("B4", "OPC_LONG_ACK", "", 2);
    _locopacket.addOPcode("B2", "OPC_INPUT_REP", "", 2);
    _locopacket.addOPcode("B1", "OPC_SW_REP", "", 2);
    _locopacket.addOPcode("B0", "OPC_SW_REQ", "", 2);
}

void MainWindow::loadOPcodeComboBox()
{
    for (int _index = 0; _index < locopacket.numOPcode(); ++_index)
    {
        ui->comboBox_opcodes->insertItem(_index, locopacket.getOPcodeName(_index));
    }
}

void MainWindow::updateOPcodeFromComboBox()
{
    QString _hex = locopacket.getOPcodeHex(ui->comboBox_opcodes->currentIndex());
    ui->lineEdit_opcode->setText(_hex);
    enableArgs();
}

void MainWindow::listSerialPorts()
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







