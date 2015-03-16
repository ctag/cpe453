#include "mainwindow.h"
#include "ui_mainwindow.h"
 MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

          //GUI

        ui->mytoolbox->setFixedWidth(220);
        ui->horizontalLayout_2->setMargin(0);
        ui->horizontalLayout_3->setMargin(0);
        ui->horizontalLayout->setMargin(0);
        ui->tabWidget->setTabText(0,"Layout Options");

        //Connect Signals to mywidget
        connect(ui->rad_track_piece,SIGNAL(clicked()),this,SLOT(check_rad()));//
        connect(ui->rad_det_node,SIGNAL(clicked()),this,SLOT(check_rad()));//
        connect(ui->rad_switch,SIGNAL(clicked()),this,SLOT(check_rad()));
        connect(this,SIGNAL(rad_track_status(bool)),ui->mytraintrack,SLOT(get_rad_rad(bool)));
        connect(this,SIGNAL(rad_det_status(bool)),ui->mytraintrack,SLOT(get_det_rad(bool)));
        connect(this,SIGNAL(rad_switch_status(bool)),ui->mytraintrack,SLOT(get_switch_rad(bool)));

  }

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::check_rad(){

    if(ui->rad_track_piece->isChecked())
        emit rad_track_status(true);
    if(ui->rad_det_node->isChecked())
        emit rad_det_status(true);
    if(ui->rad_switch->isChecked())
        emit rad_switch_status(true);
}


