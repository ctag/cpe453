#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
 * SQL Test, use this code how you may
 *
 * Conventions:
 *
 * Underscores indicate a local variable to a function
 *
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

    locosql = new SQL;
    locosql->moveToThread(&threadSQL);
    //threadSQL.setObjectName("threadSQL");

    /*
    locoudp.setParent(0);
    locoudp.moveToThread(&threadUDP);
    threadUDP.setObjectName("threadUDP");
    threadUDP.start();
    */

    //locoudp.do_openSocket(7755);

    connect(ui->pushButton_connect, SIGNAL(clicked()), this, SLOT(do_connectDB()));
    connect(ui->pushButton_disconnect, SIGNAL(clicked()), this, SLOT(do_disconnectDB()));
    connect(locosql, &SQL::DBopened, this, &MainWindow::handle_DBopened);
    connect(locosql, &SQL::DBclosed, this, &MainWindow::handle_DBclosed);
    // Kickstart thread
    threadSQL.start();
    locosql->do_run();

    if (debug) qDebug() << "Interface loaded.";
}

MainWindow::~MainWindow()
{
    // Clean up sub-threads
    threadSQL.quit();
    threadSQL.wait();
    //threadUDP.quit();
    //threadUDP.wait();

    delete ui;
}

void MainWindow::handle_DBopened()
{
    ui->textBrowser_sql->append("Database opened. Connection test appears successful :)");
    ui->pushButton_connect->setEnabled(false);
    ui->pushButton_disconnect->setEnabled(true);
    ui->pushButton_runQuery->setEnabled(true);
}

void MainWindow::handle_DBclosed()
{
    ui->textBrowser_sql->append("Database closed.");
    ui->pushButton_connect->setEnabled(true);
    ui->pushButton_disconnect->setEnabled(false);
    ui->pushButton_runQuery->setEnabled(false);
}

void MainWindow::do_connectDB()
{
    // Collect information from window
    QString hostname = ui->lineEdit_hostname->text();
    int port = ui->spinBox_port->value();
    QString database = ui->lineEdit_database->text();
    QString username = ui->lineEdit_user->text();
    QString password = ui->lineEdit_password->text();

    locosql->do_openDB(hostname, port, database, username, password);
}

void MainWindow::do_disconnectDB()
{
    locosql->do_closeDB();
}






