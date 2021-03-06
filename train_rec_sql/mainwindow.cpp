#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
 * SQL Test, use this code how you may
 *
 * Conventions:
 * Underscores indicate a local variable to a function
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

    // Declaring thread association
    locosql = new SQL;
    locosql->moveToThread(&threadSQL);

    // Database Sig/Slots
    connect(ui->pushButton_connect, SIGNAL(clicked()), this, SLOT(do_connectDB()));
    connect(ui->pushButton_disconnect, SIGNAL(clicked()), this, SLOT(do_disconnectDB()));
    connect(locosql, &SQL::DBopened, this, &MainWindow::handle_DBopened);
    connect(locosql, &SQL::DBclosed, this, &MainWindow::handle_DBclosed);
    // Handle Initializing from Sig/Slot
    //connect(ui->pushButton_thread_beginSQL, SIGNAL(clicked()), locosql, SLOT(do_run()));
    //connect(ui->pushButton_thread_beginSQL, SIGNAL(clicked(bool)), ui->pushButton_thread_beginSQL, SLOT(setEnabled(bool)));

    // Request Sig/Slots
    connect(ui->pushButton_trackReset, SIGNAL(clicked()), locosql, SLOT(req_trackReset()));
    connect(ui->pushButton_trackOn, SIGNAL(clicked()), locosql, SLOT(req_trackOn()));
    connect(ui->pushButton_trackOff, SIGNAL(clicked()), locosql, SLOT(req_trackOff()));
    connect(ui->pushButton_slotReq, SIGNAL(clicked()), this, SLOT(handle_slotReq()));
    connect(ui->pushButton_slotScan, SIGNAL(clicked()), this, SLOT(handle_slotScan()));
    connect(ui->pushButton_slotUse, SIGNAL(clicked()), this, SLOT(handle_slotUse()));
    connect(ui->pushButton_slotClear, SIGNAL(clicked()), this, SLOT(handle_slotClear()));
    connect(ui->pushButton_slotDispatch, SIGNAL(clicked()), this, SLOT(handle_slotDispatch()));
    connect(ui->pushButton_trainReq, SIGNAL(clicked()), this, SLOT(handle_trainReq()));
    connect(ui->pushButton_switchReq, SIGNAL(clicked()), this, SLOT(handle_switchReq()));

    // List Sig/Slots
    connect(ui->pushButton_getTrains, &QPushButton::clicked, locosql, &SQL::do_listTrains);
    connect(ui->pushButton_listBlocks, &QPushButton::clicked, locosql, &SQL::do_listBlocks);
    connect(locosql, &SQL::trainsList, this, &MainWindow::handle_listTrains);
    connect(locosql, &SQL::blocksList, this, &MainWindow::handle_listBlocks);

    // Kickstart thread
    threadSQL.start();
    locosql->do_run();

    qDebug() << "Interface loaded.";
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

/**
 * HELPER FUNCTIONS
 * These methods only really exist to keep this code readable
 * and make it easier for teams to reimplement it.
 */

void MainWindow::handle_slotReq()
{
    int _train = ui->spinBox_slotReq->value();
    locosql->req_slotReq(_train);
}

void MainWindow::handle_slotScan()
{
    int _train = ui->spinBox_slotMacros->value();
    locosql->req_slotScan(_train);
}

void MainWindow::handle_slotDispatch()
{
    int _train = ui->spinBox_slotMacros->value();
    locosql->req_slotDispatch(_train);
}

void MainWindow::handle_slotUse()
{
    int _train = ui->spinBox_slotMacros->value();
    locosql->req_slotUse(_train);
}

void MainWindow::handle_slotClear()
{
    int _train = ui->spinBox_slotMacros->value();
    locosql->req_slotClear(_train);
}

void MainWindow::handle_trainReq()
{
    int _slot = ui->spinBox_slotTrainReq->value();
    int _speed = ui->spinBox_setTrainSpeed->value();
    int _dir = ui->spinBox_trainDir->value();
    locosql->req_train(_slot, _speed, _dir);
}

void MainWindow::handle_switchReq()
{
    int _adr = ui->spinBox_switchAdr->value();
    int _state = ui->spinBox_switchState->value();
    locosql->req_switch(_adr, _state);
}

void MainWindow::handle_listTrains(QVector<int> _adrs, QVector<int> _slots, QVector<int> _speeds, QVector<int> _dirs, QVector<QString> _states)
{
    if (debug) qDebug() << "Loading trainList element with a new set of trains.";
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
        if (debug) qDebug() << "Train added: " << _train;
    }
}

void MainWindow::handle_listBlocks(QVector<QString> _ids, QVector<int> _positions)
{
    if (debug) qDebug() << "Loading blockList element with a new set of trains.";
    ui->plainTextEdit_listBlocks->clear();
    while (!_ids.isEmpty())
    {
        QString _block = "Block: ";
        _block.append(_ids.takeFirst());
        _block.append(" State: ");
        _block.append(QString::number(_positions.takeFirst()));
        ui->plainTextEdit_listBlocks->appendPlainText(_block);
        if (debug) qDebug() << "Block added: " << _block;
    }
}





