#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QLine"
#include "QEvent"
#include "QMoveEvent"
#include "QGraphicsSceneMouseEvent"
#include <QMouseEvent>
#include <QPoint>
#include <QPixmap>
#include <QPainter>
#include <QDebug>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QAction>
#include <QRadioButton>
#include "mywidget.h"
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
    mywidget my;
    Ui::MainWindow *ui;

        //GUI


protected:
    /*    void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);//added
        void paintEvent(QPaintEvent *paintEvent);
        void drawLines(QPainter *p);
*/

 private slots:
        void check_rad();
 signals:
        void rad_track_status(bool status);
        void rad_det_status(bool status);

};

#endif // MAINWINDOW_H
