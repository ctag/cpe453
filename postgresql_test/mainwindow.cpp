#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    /*QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::Socks5Proxy);
    proxy.setHostName("172.21.0.1");
    proxy.setPort(3128);
    QNetworkProxy::setApplicationProxy(proxy);
    */
    
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("berocs.com");
    db.setPort(5432);
    db.setDatabaseName("tmpsql");
    db.setUserName("testsql");
    db.setPassword("test");
    
    if (!db.open())
    {
        qDebug() << "Opening postgresql database failed.";
        qDebug() << db.lastError();
    } else {
        qDebug() << "Database opened.";
        
    }
    
}

MainWindow::~MainWindow()
{
    delete ui;
}
