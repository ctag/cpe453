#ifndef MYITEM_H
#define MYITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsSceneHoverEvent>
#include <QDebug>
#include <QDateTime>

 class myitem :  public QGraphicsItem
{

public:
    myitem(QPointF eventPos, int itemID);
    QPointF mypoint;
    int xposition;
    int yposition;
    QRectF boundingRect() const;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
    bool isSwitch;
    bool isNode;
    bool draw_track;
    int nodeID;
    QRectF rect;

protected:
    //void mousePressEvent(QGraphicsSceneMouseEvent *event);
    //void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    //void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void debugMsg(QString _msg);

private:
    bool doDebug;
    QDateTime dateTime;

public slots:
    void set_stuff();

signals:

 };

#endif // MYITEM_H
