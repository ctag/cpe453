#include "dialogcurved.h"
#include "ui_dialogcurved.h"

dialogcurved::dialogcurved(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogcurved)
{
    ui->setupUi(this);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(checkValues()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(nope()));//if user selects cancel, use the "nope" function

    QIntValidator* intValidator = new QIntValidator(ui->trackLength);
    ui->trackLength->setValidator(intValidator);
    QIntValidator* inkValidator = new QIntValidator(ui->curveDegree);
    ui->curveDegree->setValidator(inkValidator);

}

dialogcurved::~dialogcurved()
{
    delete ui;
}


void dialogcurved::checkValues()
{
    QString temp1 = ui->trackLength->text();
    QString temp2 = ui->curveDegree->text();
    curvedLength = temp1.toInt();
    curvedDegree = temp2.toInt();
}

void dialogcurved::nope()//is user picks cancel, make sure empty values are written to main window
{
    curvedLength = 0;
    curvedDegree = 0;
    this->rejected();
}
