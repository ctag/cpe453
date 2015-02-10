#ifndef DIALOGSTRAIGHT_H
#define DIALOGSTRAIGHT_H

#include <QDialog>

namespace Ui {
class dialogStraight;
}

class dialogStraight : public QDialog
{
    Q_OBJECT

public:
    explicit dialogStraight(QWidget *parent = 0);
    ~dialogStraight();
    int straightLength;

private:
    Ui::dialogStraight *ui;

private slots:
    void trackLength();
    void nope();
};

#endif // DIALOGSTRAIGHT_H
