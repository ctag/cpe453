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
    connect(ui->pushButton_runQuery, SIGNAL(clicked()), this, SLOT(runQuery()));
    
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
        ui->pushButton_runQuery->setEnabled(true);
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
            ui->pushButton_runQuery->setEnabled(false);
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

    // Build the query from text entered in MainWindow.
    // Match the form "SELECT * FROM schema.table" to agree with Postgresql.
    schema = ui->lineEdit_schema->text();
    table = ui->lineEdit_table->text();
    QString tmpQuery = "SELECT * FROM ";
    tmpQuery.append(schema);
    tmpQuery.append(".");
    tmpQuery.append(table);

    // Load our query text into a query model
    dbQueryModel->setQuery(tmpQuery, db);

    // Check if the query model is correct
    if (dbQueryModel->lastError().isValid())
    {
        ui->textBrowser_output->append(dbQueryModel->lastError().text());
        ui->textBrowser_output->append("Issue with dbQueryModel listed above, exiting queryModel().");
        return;
    }

    // Load the query model into the table view. This is an attempt at View/Model practices.
    dbTableView->setModel(dbQueryModel);
    dbTableView->show();
}

void MainWindow::runQuery()
{
    QString queryString = ui->lineEdit_query->text(); /* Pull fresh query from the main interface */
    if (!db.isOpen()) /* Check that the database is still open */
    {
        ui->textBrowser_output->append("Database doesn't appear to be open :C");
        return;
    } else if (dbQuery.exec(queryString)) /* Execute the query and check that it succeeds */
    {
        ui->textBrowser_output->append("Query successful? :3");
        if (dbQuery.isSelect()) /* A SELECT query was run */
        {
            ui->textBrowser_output->append("Detected that the query is type SELECT.");
            while (dbQuery.next()) /* Must prime the results by calling .next(); the first row is always null */
            {
                int queryValue = 0;
                QString queryResults = "";
                while (!dbQuery.isNull(queryValue)) /* Cycle through the columns */
                {
                    queryResults.append("[");
                    queryResults.append(QString("%1").arg(queryValue)); /* Cast the int as QString */
                    queryResults.append("]: ");
                    queryResults.append(dbQuery.value(queryValue).toString());
                    queryValue++; /* Go to next column in row */
                }
                ui->textBrowser_output->append(queryResults);
            }
        } else { /* Query was not type SELECT */
            ui->textBrowser_output->append("Detected that the query is NOT type SELECT.");
            ui->textBrowser_output->append("Number of rows affected: " + QString("%1").arg(dbQuery.numRowsAffected()));
        }
    } else { /* The query was not successful */
        ui->textBrowser_output->append("Dun goofed. This shouldn't happen.");
        ui->textBrowser_output->append(dbQuery.lastError().text());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
