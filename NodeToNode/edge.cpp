#include "edge.h"
edge:: edge(QPointF beginPos,QPointF finalPos)
{
   linestartPos=beginPos;
   lineendPos=finalPos;
    setFlag(ItemIsSelectable);
}


QRectF edge::boundingRect() const
{
    return QRectF(0,0,30,40);
}

void edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(Qt::black,4));
    painter->drawLine(linestartPos,lineendPos);
}

void edge::updatePosition()
{

}

void edge::mousePressEvent(QGraphicsSceneMouseEvent *event){

    isSelected();
    QGraphicsLineItem::mousePressEvent(event);
}
