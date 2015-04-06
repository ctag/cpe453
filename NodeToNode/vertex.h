#ifndef VERTEX_H
#define VERTEX_H

#include <QGraphicsItem>
#include <QPainter>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsSceneHoverEvent>
#include <QDebug>
#include <QDateTime>

 class vertex :  public QGraphicsItem
{

public:
    vertex(QPointF eventPos, int itemID);
    ~vertex();
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

#endif // VERTEX_H
