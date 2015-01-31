#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QtSql>
#include <QTableView>
#include <QDebug>
#include <QtDebug>
#include <QNetworkProxy>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
    void connectDB();
    void disconnectDB();
    void tableText();
    void queryModel();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QString hostname;
    QString username;
    QString password;
    QString database;
    QString table;
    int port;
    QSqlQuery dbQuery;
    QSqlQueryModel* dbQueryModel;
    QTableView* dbTableView;

};

#endif // MAINWINDOW_H
