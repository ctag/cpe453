#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // create List

    // Declaring thread association
    locosql = new SQL;
    locosql->moveToThread(&threadSQL);

    ui->horizontalLayout_3->setMargin(0);
    ui->horizontalLayout->setMargin(0);

    // Database Sig/Slots
    connect(ui->pushButton_connect_2, SIGNAL(clicked()), this, SLOT(do_connectDB()));
    connect(ui->pushButton_disconnect_2, SIGNAL(clicked()), this, SLOT(do_disconnectDB()));
    connect(locosql, &SQL::DBopened, this, &MainWindow::handle_DBopened);
    connect(locosql, &SQL::DBclosed, this, &MainWindow::handle_DBclosed);
    connect(ui->pushButton_sqlUpload, SIGNAL(clicked()), this, SLOT(do_sqlUpload()));
    connect(ui->pushButton_sqlClear, SIGNAL(clicked()), locosql, SLOT(do_clearVertices()));

    connect(ui->mytraintrack, SIGNAL(connectedVertices(vertex*,QList<vertex*>)), locosql, SLOT(do_uploadVertex(vertex*,QList<vertex*>)));

    //connect(ui->rad_track_piece,SIGNAL(clicked()),this,SLOT(check_rad()));
    //connect(this,SIGNAL(rad_track_status(bool)),ui->mytraintrack,SLOT(get_track_rad(bool)));
    connect(ui->connectButton,SIGNAL(clicked()),ui->mytraintrack,SLOT(connect_button_clicked()));
    connect(ui->deleteButton,SIGNAL(clicked()),ui->mytraintrack,SLOT(delete_button_clicked()));
    connect(ui->switchButton,SIGNAL(clicked()),ui->mytraintrack,SLOT(switch_button_clicked()));
    connect(ui->groupButton,SIGNAL(clicked()),ui->mytraintrack,SLOT(group_button_clicked()));

    // Attach arrow buttons to track panning
    connect(ui->pushButton_shiftUp, SIGNAL(clicked()), ui->mytraintrack, SLOT(shift_up()));
    connect(ui->pushButton_shiftLeft, SIGNAL(clicked()), ui->mytraintrack, SLOT(shift_left()));
    connect(ui->pushButton_shiftDown, SIGNAL(clicked()), ui->mytraintrack, SLOT(shift_down()));
    connect(ui->pushButton_shiftRight, SIGNAL(clicked()), ui->mytraintrack, SLOT(shift_right()));

    // Kickstart thread
    threadSQL.start();
    locosql->do_run();
}

MainWindow::~MainWindow()
{
    // Clean up sub-threads
    threadSQL.quit();
    threadSQL.wait();

    delete ui;
}

/**
 * Database Methods
 */

void MainWindow::handle_DBopened()
{
    ui->textBrowser_sql_2->append("Database opened. Connection appears successful :)");
    ui->pushButton_connect_2->setEnabled(false);
    ui->pushButton_disconnect_2->setEnabled(true);
    ui->pushButton_sqlUpload->setEnabled(true);
    ui->pushButton_sqlClear->setEnabled(true);
}

void MainWindow::handle_DBclosed()
{
    ui->textBrowser_sql_2->append("Database closed.");
    ui->pushButton_connect_2->setEnabled(true);
    ui->pushButton_disconnect_2->setEnabled(false);
    ui->pushButton_sqlUpload->setEnabled(false);
    ui->pushButton_sqlClear->setEnabled(false);
}

void MainWindow::do_connectDB()
{
    // Collect information from window
    QString hostname = ui->lineEdit_hostname_2->text();
    int port = ui->spinBox_port_2->value();
    QString database = ui->lineEdit_database_2->text();
    QString username = ui->lineEdit_user_2->text();
    QString password = ui->lineEdit_password_2->text();

    locosql->do_openDB(hostname, port, database, username, password);
}

void MainWindow::do_disconnectDB()
{
    locosql->do_closeDB();
}

void MainWindow::do_sqlUpload()
{
    ui->mytraintrack->get_allConnectedVertices();
}

/*void MainWindow::check_rad(){

   if(ui->rad_track_piece->isChecked())
       emit rad_track_status(true);
   else
       emit rad_track_status(false);

}*/


