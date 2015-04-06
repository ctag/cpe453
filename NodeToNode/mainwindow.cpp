#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //create List

    //GUI

    ui->horizontalLayout_3->setMargin(0);
    ui->horizontalLayout->setMargin(0);
    ui->tabWidget->setTabText(0,"Layout Options");


    connect(ui->rad_track_piece,SIGNAL(clicked()),this,SLOT(check_rad()));//
    connect(this,SIGNAL(rad_track_status(bool)),ui->mytraintrack,SLOT(get_track_rad(bool)));
    connect(ui->deleteButton,SIGNAL(clicked()),ui->mytraintrack,SLOT(delete_button_clicked()));
    connect(ui->switchButton,SIGNAL(clicked()),ui->mytraintrack,SLOT(switch_button_clicked()));
    connect(ui->nodeButton,SIGNAL(clicked()),ui->mytraintrack,SLOT(node_button_clicked()));


}

   MainWindow::~MainWindow()
   {
       delete ui;
   }


   void MainWindow::check_rad(){

       if(ui->rad_track_piece->isChecked())
           emit rad_track_status(true);
       else
           emit rad_track_status(false);

   }


