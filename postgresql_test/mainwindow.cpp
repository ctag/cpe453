#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    connect(ui->pushButton_connect, SIGNAL(clicked()), this, SLOT(connectDB()));

    
}

void MainWindow::connectDB()
{
    hostname = ui->lineEdit_hostname->text();
    port = ui->spinBox_port->value();
    database = ui->lineEdit_database->text();
    username = ui->lineEdit_user->text();
    password = ui->lineEdit_password->text();
    table = ui->lineEdit_table->text();

    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(hostname);
    db.setPort(port);
    db.setUserName(username);
    db.setPassword(password);
    db.setDatabaseName(database);


    if (!db.open())
    {
        ui->textBrowser_output->append("Opening postgresql database failed.");
        ui->textBrowser_output->append(db.lastError().text());
    } else {
        ui->textBrowser_output->append("Database opened.");
    }

    db.close();
    ui->textBrowser_output->append("Database closed.");
}

MainWindow::~MainWindow()
{
    delete ui;
}
