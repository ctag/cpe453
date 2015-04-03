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
    connect(ui->pushButton_trackReset, SIGNAL(clicked()), locosql, SLOT(do_trackReset()));
    connect(ui->pushButton_trackOn, SIGNAL(clicked()), locosql, SLOT(do_trackOn()));
    connect(ui->pushButton_trackOff, SIGNAL(clicked()), locosql, SLOT(do_trackOff()));
    connect(ui->pushButton_slotReq, SIGNAL(clicked()), this, SLOT(handle_slotReq()));
    connect(ui->pushButton_slotScan, SIGNAL(clicked()), this, SLOT(handle_slotScan()));
    connect(ui->pushButton_slotUse, SIGNAL(clicked()), this, SLOT(handle_slotUse()));
    connect(ui->pushButton_slotClear, SIGNAL(clicked()), this, SLOT(handle_slotClear()));
    connect(ui->pushButton_slotDispatch, SIGNAL(clicked()), this, SLOT(handle_slotDispatch()));
    connect(ui->pushButton_setTrainSpeed, SIGNAL(clicked()), this, SLOT(handle_reqTrain()));
    connect(ui->pushButton_switchSet, SIGNAL(clicked()), this, SLOT(handle_reqSwitch()));
    connect(ui->pushButton_getTrains, &QPushButton::clicked, locosql, &SQL::do_listTrains);
    connect(ui->pushButton_listBlocks, &QPushButton::clicked, locosql, &SQL::do_listBlocks);
    connect(locosql, &SQL::trainList, this, &MainWindow::handle_listTrains);
    connect(locosql, &SQL::blockList, this, &MainWindow::handle_listBlocks);
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

void MainWindow::handle_slotReq()
{
    int _train = ui->spinBox_slotReq->value();
    locosql->do_slotReq(_train);
}

void MainWindow::handle_slotScan()
{
    int _train = ui->spinBox_slotMacros->value();
    locosql->do_slotScan(_train);
}

void MainWindow::handle_slotDispatch()
{
    int _train = ui->spinBox_slotMacros->value();
    locosql->do_slotDispatch(_train);
}

void MainWindow::handle_slotUse()
{
    int _train = ui->spinBox_slotMacros->value();
    locosql->do_slotUse(_train);
}

void MainWindow::handle_slotClear()
{
    int _train = ui->spinBox_slotMacros->value();
    locosql->do_slotClear(_train);
}

void MainWindow::handle_reqTrain()
{
    int _slot = ui->spinBox_slotTrainReq->value();
    int _speed = ui->spinBox_setTrainSpeed->value();
    int _dir = ui->spinBox_trainDir->value();
    locosql->do_reqTrain(_slot, _speed, _dir);
}

void MainWindow::handle_reqSwitch()
{
    int _adr = ui->spinBox_switchAdr->value();
    int _state = ui->spinBox_switchState->value();
    locosql->do_reqSwitch(_adr, _state);
}

void MainWindow::handle_listTrains(QVector<int> _adrs, QVector<int> _slots, QVector<int> _speeds, QVector<int> _dirs, QVector<QString> _states)
{
    qDebug() << "Loading trainList element with a new set of trains.";
    ui->plainTextEdit_listTrains->clear();
    while (!_slots.isEmpty())
    {
        QString _train = "Train: ";
        _train.append(QString::number(_adrs.takeFirst()));
        _train.append("Slot: ");
        _train.append(QString::number(_slots.takeFirst()));
        _train.append("; Speed: ");
        _train.append(QString::number(_speeds.takeFirst()));
        _train.append("; Dir: ");
        _train.append(QString::number(_dirs.takeFirst()));
        _train.append("; State: ");
        _train.append(_states.takeFirst());
        ui->plainTextEdit_listTrains->appendPlainText(_train);
        qDebug() << "Train added: " << _train;
    }
}

void MainWindow::handle_listBlocks(QVector<int> _ids, QVector<int> _positions)
{
    qDebug() << "Loading blockList element with a new set of trains.";
    ui->plainTextEdit_listBlocks->clear();
    while (!_ids.isEmpty())
    {
        QString _block = "Block: ";
        _block.append(QString::number(_ids.takeFirst()));
        _block.append(" State: ");
        _block.append(QString::number(_positions.takeFirst()));
        ui->plainTextEdit_listBlocks->appendPlainText(_block);
        qDebug() << "Block added: " << _block;
    }
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

void MainWindow::handle_setTrain()
{
    //
}





