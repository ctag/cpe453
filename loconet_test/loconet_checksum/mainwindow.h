#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QSerialPort>
#include <QDebug>
#include <QByteArray>
#include <QVariant>
#include <QComboBox>

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
    void genChecksum();
    void loadOPCode(int index);

private:
    Ui::MainWindow *ui;
    QSerialPort *loconet;
    //void genBitArray(QByteArray bytes, QBitArray* bits);
    QBitArray genBitArray(QByteArray bytes);
    QBitArray doXor(QBitArray ar1, QBitArray ar2);
};

#endif // MAINWINDOW_H
