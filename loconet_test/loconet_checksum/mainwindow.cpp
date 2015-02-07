#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    locopacket = new LocoPacket("83 7C");

    ui->textBrowser_console->append(usbPorts.availablePorts().first().portName().toLatin1());
    ui->textBrowser_console->append(usbPorts.description());

    ui->lineEdit_opcode->setInputMask("hh");
    ui->lineEdit_arg1->setInputMask("hh");
    ui->lineEdit_arg2->setInputMask("hh");
    ui->lineEdit_opcode->setText("");

    ui->lineEdit_chk->setEnabled(false);

    loconet = new QSerialPort;

    connect(ui->pushButton_genPacket, SIGNAL(clicked()), this, SLOT(genPacket()));
    connect(ui->lineEdit_opcode, SIGNAL(returnPressed()), this, SLOT(enableArgs()));

    ui->comboBox_opcodes->setEditable(false);
    ui->comboBox_opcodes->setInsertPolicy(QComboBox::InsertAtBottom);

    ui->comboBox_opcodes->insertItem(0, "Global Power On");
    ui->comboBox_opcodes->insertItem(1, "Global Power Off");
    ui->comboBox_opcodes->insertItem(2, "Global IDLE");
    ui->comboBox_opcodes->insertItem(3, "Master Busy");

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
    LocoPacket * _packet;
    _hex.append(ui->lineEdit_opcode->text());
    _hex.append(ui->lineEdit_arg1->text());
    _hex.append(ui->lineEdit_arg2->text());

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
}

/* Flippity Bit
 * That code was stale as
 */







