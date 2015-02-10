#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QLabel>
#include <QDialog>
#include "dialogstraight.h"
#include "dialogcurved.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QAction* straightTrack;
    QAction* curvedTrack;
    QAction* connectorTrack;
    QAction* bridgeTrack;

    QAction* closeOut;

    QMenu* menu;

private slots:
    void straightTrackPiece();
    void curvedTrackPiece();
    void connectorTrackPiece();
    void bridgeTrackPiece();

};

#endif // MAINWINDOW_H
