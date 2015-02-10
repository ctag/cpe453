#ifndef DIALOGCURVED_H
#define DIALOGCURVED_H

#include <QDialog>

namespace Ui {
class dialogcurved;
}

class dialogcurved : public QDialog
{
    Q_OBJECT

public:
    explicit dialogcurved(QWidget *parent = 0);
    ~dialogcurved();
    int curvedLength;
    int curvedDegree;

private:
    Ui::dialogcurved *ui;

private slots:
    void checkValues();
    void nope();
};

#endif // DIALOGCURVED_H
