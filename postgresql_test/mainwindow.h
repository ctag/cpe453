#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QtSql>
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

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QString hostname;
    QString username;
    QString password;
    QString database;
    QString table;
    int port;

};

#endif // MAINWINDOW_H
