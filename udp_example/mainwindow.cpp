#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton_connect, SIGNAL(clicked()), this, SLOT(do_connectUDP()));
    connect(ui->pushButton_send, SIGNAL(clicked()), this, SLOT(do_sendUDP()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_connectUDP()
{
    if (ui->pushButton_connect->text() == "connect")
    {
        QHostAddress _addr;
        _addr.setAddress(ui->lineEdit_ipaddress->text().toInt());
        quint16 _port = ui->spinBox_port->value();
        socket.close();
        socket.bind(_addr, _port);
        if (socket.isOpen() && socket.isWritable())
        {
            ui->pushButton_connect->setText("disconnect");
        } else {
            socket.close();
        }
    } else {
        socket.close();
        ui->pushButton_connect->setText("connect");
    }
}

void MainWindow::do_sendUDP()
{
    QHostAddress _addr;
    _addr.setAddress(ui->lineEdit_ipaddress->text().toInt());
    quint16 _port = ui->spinBox_port->value();
    QByteArray _data(ui->lineEdit_message->text().toUtf8());
    socket.writeDatagram(_data, _data.size(), _addr, _port);
}
