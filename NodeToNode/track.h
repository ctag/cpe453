#ifndef TRACK_H
#define TRACK_H

#include <QLabel>
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
#include <QRadioButton>
#include <QList>
#include <QDateTime>

#include "vertex.h"
#include "edge.h"

class track : public QGraphicsView
{
    Q_OBJECT

public:
    explicit track(QWidget *parent = 0);
    ~track();
    vertex *previousNode;
    vertex *activeVertex;
    int activeItemID;
    QGraphicsItem* selectedNode;
    int id_counter;
    QGraphicsScene* scene;
    bool track_rad_state;
    QPointF startPos;
    QPointF endPos;
    QList<vertex*> nodeList;
    QGraphicsLineItem * line;

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
    void delete_button_clicked();
    void switch_button_clicked();
    void node_button_clicked();

signals:
 //   void positionChange(QPointF );
};

#endif // TRACK_H
