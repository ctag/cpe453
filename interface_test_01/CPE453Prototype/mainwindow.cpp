#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>
#include <QFileDialog>
#include <QDialog>
#include <Qt>
#include <QProgressDialog>
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QTransform>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    straightTrack = new QAction("Straight Track", this);//gives loadTrack action a name (the one in quoutes) and creates it
    connect(straightTrack, SIGNAL(triggered()), this, SLOT(straightTrackPiece()));//connects our private slot function loadLabelImage to the trigger of the loadImage action
    //loadTrack->setShortcuts(QKeySequence::Open);//makes control o a shortcut for activating loadImage

    curvedTrack = new QAction("Curved Track", this);
    connect(curvedTrack, SIGNAL(triggered()), this, SLOT(curvedTrackPiece()));

    connectorTrack = new QAction("Connector Track", this);
    connect(connectorTrack, SIGNAL(triggered()), this, SLOT(connectorTrackPiece()));

    bridgeTrack = new QAction("Bridge Track", this);
    connect(bridgeTrack, SIGNAL(triggered()), this, SLOT(bridgeTrackPiece()));

    closeOut = new QAction("Quit", this);
    connect(closeOut, SIGNAL(triggered()), this, SLOT(close()));

    //need a loadmap action under the file menu so the user can bring up the pre-defined track layout

    menu = menuBar()->addMenu("File");
    menu->addAction(closeOut);
    menu = menuBar()->addMenu("Track");//adds the drop-down menu labeled "File" to the mainwindow.
    menu->addAction(straightTrack);//adds StraightTrack and its text to the list of menu "File"
    menu->addSeparator();
    menu->addAction(curvedTrack);
    menu->addSeparator();
    menu->addAction(connectorTrack);
    menu->addSeparator();
    menu->addAction(bridgeTrack);

    QGraphicsScene scene;
    QGraphicsRectItem *rect = scene.addRect(QRectF(0, 0, 100, 100));
    //QGraphicsItem *item = scene.itemAt(50, 50);

    //QGraphicsView view(&scene);
    //view.show();
    ui->graphicsView->setScene(&scene);
    ui->graphicsView->show();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::straightTrackPiece()//asks for parameters for the specififed track type
{
    dialogStraight stp;
    stp.exec();
    int value = stp.dialogStraight::straightLength;//grabs the result of the pop-up dialog
    QString carry = QString::number(value, 10);
    ui->tester->setText(carry);
}

void MainWindow::curvedTrackPiece()
{
    dialogcurved ctp;
    ctp.exec();
    int length = ctp.dialogcurved::curvedLength;
    int curve = ctp.dialogcurved::curvedDegree;
    QString carry = QString::number(length, 10);
    QString moar = QString::number(curve, 10);
    carry.append(" ");
    carry = carry + moar;
    ui->tester->setText(carry);
}

void MainWindow::connectorTrackPiece()
{

}

void MainWindow::bridgeTrackPiece()
{

}
