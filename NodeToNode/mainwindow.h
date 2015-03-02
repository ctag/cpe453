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

 namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool track_rad_state;
    bool detection_rad_state;
    bool switch_rad_state;

private:

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
     //   void check_rad();
 signals:
        //void radstatus(bool);
};

#endif // MAINWINDOW_H
