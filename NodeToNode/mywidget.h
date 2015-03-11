#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QLabel>
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
#include <QList>
//#include "mainwindow.h"
  class mywidget : public QLabel
{
    Q_OBJECT
public:
    explicit mywidget(QWidget *parent = 0);
      ~mywidget();


private:
    QPoint startPos;
    QPoint endPos;
    QPoint switch_startPos;
    QPoint switch_endPos;
    bool firstclick;
    bool secondclick;
    bool originclick;
    bool isSwitch;



    bool leftDown;
    bool rightDown;
    bool connectsToPrevious;
    QVector<QLine> lines;

    bool track_rad_state;
    bool detection_rad_state;
    bool switch_rad_state;
    //QLinkedList<int> mypoints;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *paintEvent);
    void drawLines(QPainter *p);

public slots:
    void get_rad_rad(bool status);
    void get_det_rad(bool status);
    void get_switch_rad(bool status);
signals:
   // void request_rad_status(bool);

};

#endif // MYWIDGET_H
