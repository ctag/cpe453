#include "edge.h"

edge::edge(QPointF beginPos,QPointF finalPos)
{
    setFlag(ItemIsSelectable,true);
    setFlag(ItemIsMovable,true);

    linestartPos=beginPos;
    lineendPos=finalPos;
}

void edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setPen(QPen(Qt::black,4));
    painter->drawLine(linestartPos,lineendPos);

}

void edge::updatePosition(){

}
