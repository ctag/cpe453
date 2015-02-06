#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    locopacket = new LocoPacket("01FE");

    ui->textBrowser_console->append(usbPorts.availablePorts().first().portName().toLatin1());
    ui->textBrowser_console->append(usbPorts.description());

    ui->lineEdit_opcode->setInputMask("HH");
    ui->lineEdit_arg1->setInputMask("hh");
    ui->lineEdit_arg2->setInputMask("hh");

    ui->lineEdit_chk->setEnabled(false);

    loconet = new QSerialPort;

    connect(ui->pushButton_genChk, SIGNAL(clicked()), this, SLOT(genChecksum()));
    connect(ui->comboBox_opcodes, SIGNAL(activated(int)), this, SLOT(loadOPCode(int)));

    ui->comboBox_opcodes->setEditable(false);
    ui->comboBox_opcodes->setInsertPolicy(QComboBox::InsertAtBottom);

    ui->comboBox_opcodes->insertItem(0, "Global Power On");
    ui->comboBox_opcodes->insertItem(1, "Global Power Off");
    ui->comboBox_opcodes->insertItem(2, "Global IDLE");
    ui->comboBox_opcodes->insertItem(3, "Master Busy");

    ui->textBrowser_console->append("Program loaded! :3");
}

void MainWindow::loadOPCode(int index)
{
    switch (index)
    {
        case 0:
            ui->lineEdit_opcode->setText("83"); /* Global ON */
        break;
        case 1:
            ui->lineEdit_opcode->setText("82"); /* Global OFF */
        break;
        case 2:
            ui->lineEdit_opcode->setText("85"); /* Force IDLE */
        break;
        case 3:
            ui->lineEdit_opcode->setText("81"); /* MASTER busy */
        break;
        default:
            ui->textBrowser_console->append("Whoops, error loading the OP code (-_-)");
            ui->lineEdit_opcode->setText("00"); /* General Error */
        break;
    }
}

QBitArray MainWindow::genBitArray(QByteArray bytes)
{
    QBitArray bits(bytes.count()*8);
    //ui->textBrowser_console->append("Array bytes: " + bytes.count());
    for (int i = 0; i < bytes.count(); i++)
    {
        for (int b = 0; b < 8; b++)
        {
            bits.setBit((i*8)+b, bytes.at(i)&(1<<(7-b)));
        }
    }
    QString textOutput = "BitArray Generated (^_^) ";
    textOutput.append(bytes.toHex());
    ui->textBrowser_console->append(textOutput);
    return(bits);
}

QBitArray MainWindow::doXor(QBitArray ar1, QBitArray ar2)
{
    if (ar1.size() != ar2.size())
    {
        ui->textBrowser_console->append("bitarray size doesn't match, exiting doXor.");
        QBitArray youlose(0);
        return youlose;
    }
    QBitArray result(ar1.size());
    for (int i = 0; i < ar1.size(); i++)
    {
            result.setBit(i, ar1[i]^ar2[i]);
    }
    ui->textBrowser_console->append("BitArray XOR'ed x) ");
    return result;
}

void MainWindow::genChecksum()
{
    int packet_len = 0;
    QString* opcode_text = new QString;
    QString* arg1_text = new QString;
    QString* arg2_text = new QString;
    QByteArray conversion_data;
    QByteArray opcode_hex;
    QByteArray arg1_hex;
    QByteArray arg2_hex;
    QBitArray opcode_bits;
    QBitArray arg1_bits;
    QBitArray arg2_bits;
    QBitArray checksum;


    if (ui->lineEdit_opcode->hasAcceptableInput())
    {
        ui->textBrowser_console->append("opcode is valid :D");
        packet_len++;
    } else {
        ui->textBrowser_console->append("opcode is not valid :C");
        return;
    }

    opcode_text->append(ui->lineEdit_opcode->text());
    arg1_text->append(ui->lineEdit_arg1->text());
    arg2_text->append(ui->lineEdit_arg2->text());

    conversion_data = opcode_text->toLatin1();
    opcode_hex = QByteArray::fromHex(conversion_data);
    conversion_data = arg1_text->toLatin1();
    arg1_hex = QByteArray::fromHex(conversion_data);
    conversion_data = arg2_text->toLatin1();
    arg2_hex = QByteArray::fromHex(conversion_data);

    opcode_bits = genBitArray(opcode_hex);
    arg1_bits = genBitArray(arg1_hex);
    arg2_bits = genBitArray(arg2_hex);

    checksum = doXor(opcode_bits, arg1_bits);
    checksum = doXor(checksum, arg2_bits);

    qDebug() << opcode_bits << arg1_bits << arg2_bits << checksum << ~checksum;

    checksum = ~checksum;

    QBitArray tmp_bits = checksum;

    qDebug() << "checksum: " << tmp_bits;

    for (int c = 0; c < 8; c++)
    {
        checksum.setBit(c, tmp_bits.at(7-c));
    }

    QByteArray checksum_bytes;

    for(int b=0; b<checksum.count(); ++b)
    {
        if (!checksum_bytes[b/8])
        {
            checksum_bytes[b/8] = NULL;
        }
        checksum_bytes[b/8] = (checksum_bytes.at(b/8) | ((checksum[b]?1:0)<<(b%8)));
    }

    qDebug() << genBitArray(checksum_bytes);

    ui->lineEdit_chk->setText(checksum_bytes.toHex());

    QString finalPacket = "Packet: ";
    finalPacket.append(opcode_text);
    finalPacket.append(" ");

    if (arg1_text != QString::fromLatin1("00"))
    {
        finalPacket.append(arg1_text);
        finalPacket.append(" ");
    }
    if (arg2_text != QString::fromLatin1("00"))
    {
        finalPacket.append(arg2_text);
        finalPacket.append(" ");
    }
    finalPacket.append(checksum_bytes.toHex());

    ui->textBrowser_console->append(finalPacket);

}

MainWindow::~MainWindow()
{
    delete ui;
}
