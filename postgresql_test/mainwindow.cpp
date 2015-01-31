#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // Initialize Postgres driver once
    db = QSqlDatabase::addDatabase("QPSQL", "testing");
    dbQuery = QSqlQuery(db);
    dbQueryModel = new QSqlQueryModel;
    dbTableView = new QTableView;

    // Link pushbuttons to class members
    connect(ui->pushButton_connect, SIGNAL(clicked()), this, SLOT(connectDB()));
    connect(ui->pushButton_disconnect, SIGNAL(clicked()), this, SLOT(disconnectDB()));
    connect(ui->pushButton_tableText, SIGNAL(clicked()), this, SLOT(tableText()));
    connect(ui->pushButton_queryModel, SIGNAL(clicked()), this, SLOT(queryModel()));
    
}

void MainWindow::connectDB()
{
    // Collect information from window
    hostname = ui->lineEdit_hostname->text();
    port = ui->spinBox_port->value();
    database = ui->lineEdit_database->text();
    username = ui->lineEdit_user->text();
    password = ui->lineEdit_password->text();
    table = ui->lineEdit_table->text();

    // Load connection details
    db.setHostName(hostname);
    db.setPort(port);
    db.setUserName(username);
    db.setPassword(password);
    db.setDatabaseName(database);

    // Attempt to open database
    if (!db.open())
    {
        ui->textBrowser_output->append("Opening postgresql database failed.");
        ui->textBrowser_output->append(db.lastError().text());
    } else {
        ui->textBrowser_output->append("Database opened. Connection test appears successful :)");
        QSqlQuery tmpq(db);
        if (!tmpq.exec("SELECT * FROM public.testsql"))
        {
            qDebug() << tmpq.lastError();
            return;
        }
        while (tmpq.next())
        {
            qDebug() << tmpq.value(0).toString();
        }
        ui->pushButton_connect->setEnabled(false);
        ui->pushButton_disconnect->setEnabled(true);
        ui->pushButton_tableText->setEnabled(true);
        ui->pushButton_queryModel->setEnabled(true);
    }

}

void MainWindow::disconnectDB()
{
    if (!db.isOpen())
    {
        ui->textBrowser_output->append("Database appears to already be closed? Wat do?!");
        return;
    } else {
        ui->textBrowser_output->append("Trying to close the database.");
        db.close();
        if (!db.isOpen())
        {
            ui->textBrowser_output->append("Database closed.");
            ui->pushButton_connect->setEnabled(true);
            ui->pushButton_disconnect->setEnabled(false);
            ui->pushButton_tableText->setEnabled(false);
            ui->pushButton_queryModel->setEnabled(false);
        }
    }
}

void MainWindow::tableText()
{
    if (!db.isOpen())
    {
        ui->textBrowser_output->append("The database isn't open.. :(");
    }

    ui->textBrowser_output->append("viewTable(): dumping table to this text box.");

    //INSERT INTO public.testsql VALUES('hi', 1);
    if (!dbQuery.exec("SELECT * FROM public.testsql"))
    {
        ui->textBrowser_output->append("Query failed :(");
        ui->textBrowser_output->append(dbQuery.lastError().text());
        return;
    }
    while (dbQuery.next())
    {
        for (int i = 0; i < 2; i++)
        {
            ui->textBrowser_output->append(dbQuery.value(i).toString());
        }
    }
    ui->textBrowser_output->append("viewTable(): done.");
}

void MainWindow::queryModel()
{
    ui->textBrowser_output->append("queryModel(): opening query view.");
    //QSqlQueryModel dbQueryModel;
    dbQueryModel->setQuery("SELECT * FROM public.testsql", db);
    if (dbQueryModel->lastError().isValid())
    {
        ui->textBrowser_output->append(dbQueryModel->lastError().text());
        ui->textBrowser_output->append("Issue with dbQueryModel listed above, exiting queryModel().");
        return;
    }
    //QTableView dbTableView;
    dbTableView->setModel(dbQueryModel);
    dbTableView->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
