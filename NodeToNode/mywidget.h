#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QLabel>
#include <QLine>
#include <QEvent>
#include <QMoveEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QGraphicsLineItem>
#include <QPoint>
#include <QGraphicsView>
#include <QPainter>
#include <QDebug>
#include <QDialog>
#include <QLine>
#include <QVBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include <QList>
#include <QDateTime>

#include "myitem.h"

class mywidget : public QGraphicsView
{
    Q_OBJECT

public:
    explicit mywidget(QWidget *parent = 0);
    ~mywidget();
    myitem *previousNode;
    myitem *activeNode;
    int activeItemID;
    QGraphicsItem* selectedNode;
    int id_counter;
    QGraphicsScene* scene;
    bool track_rad_state;
    QPointF startPos;
    QPointF endPos;
    QList<myitem*> nodeList;
    QGraphicsLineItem *line;
    QLineF *testline;

private:
    bool firstclick;
    bool secondclick;
    bool originclick;
    bool leftDown;
    bool rightDown;
    bool connectsToPrevious;
    bool doDebug;
    QDateTime dateTime;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void debugMsg(QString _msg);

public slots:
    void get_track_rad(bool status);
    //void get_det_rad(bool status);
    //void get_switch_rad(bool status);
    void delete_button_clicked();
    void switch_button_clicked();
    void node_button_clicked();

signals:

};

#endif // MYWIDGET_H
