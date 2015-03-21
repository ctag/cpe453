#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QHostAddress>
#include <QByteArray>
#include <QBitArray>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void do_connectUDP();
    void do_sendUDP();

private:
    Ui::MainWindow *ui;
    QUdpSocket socket;
};

#endif // MAINWINDOW_H
