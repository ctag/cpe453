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
    // ui->horizontalLayout_4->setMargin(0);
      ui->tabWidget->setTabText(0,"Layout Options");
    // ui->mytraintrack->setDisabled(true);
       //connect(ui->rad_track_piece,SIGNAL(clicked()),this,SLOT(check_rad));
       //connect(ui->mytraintrack,SIGNAL(request_rad_status(bool)),this, SLOT(check_rad()));
}

MainWindow::~MainWindow()
{
    delete ui;
}




