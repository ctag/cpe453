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
  class mywidget : public QLabel
{
    Q_OBJECT
public:
    explicit mywidget(QWidget *parent = 0);
      ~mywidget();


private:
    QPoint startPos;
    QPoint endPos;

    bool leftDown;
    bool rightDown;
    bool connectsToPrevious;
    QVector<QLine> lines;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *paintEvent);
    void drawLines(QPainter *p);

public slots:
  //  void rad_track_status();
signals:
    void request_rad_status(bool);

};

#endif // MYWIDGET_H
