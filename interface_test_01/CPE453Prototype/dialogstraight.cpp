#include "dialogstraight.h"
#include "ui_dialogstraight.h"
#include <QApplication>
#include <Qt>

dialogStraight::dialogStraight(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogStraight)
{
    ui->setupUi(this);
    //this->show();
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(trackLength()));//if user clicks the "okay" button, the function is called
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(nope()));//if user selects cancel, dialog closes

    //this ensures that the dialog will only accept integers.
    QIntValidator* intValidator = new QIntValidator(ui->straightTrackLength);
    ui->straightTrackLength->setValidator(intValidator);


}

dialogStraight::~dialogStraight()
{
    delete ui;
}

void dialogStraight::trackLength()
{
    QString temp = ui->straightTrackLength->text();
    straightLength = temp.toInt();
}


void dialogStraight::nope()
{
    straightLength = 0;\
    this->rejected();
}
