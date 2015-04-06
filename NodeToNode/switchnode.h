#ifndef SWITCHNODE_H
#define SWITCHNODE_H

#include <QGraphicsItem>

class switchNode : public QGraphicsItem
{
    Q_OBJECT
public:
     switchNode(QPointF eventPos, int itemID);
     QPointF mypoint;
     int xposition;
     int yposition;
     QRectF boundingRect() const;
     void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);


     bool isSwitch;
     int nodeID;
     QRectF rect;

 protected:
   void mousePressEvent(QGraphicsSceneMouseEvent *event);
   void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
   void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

 public slots:
     void set_stuff();
 signals:
  };

 #endif
