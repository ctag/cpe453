#include "switchnode.h"

switchNode::switchNode(QObject *parent) :
    QGraphicsItem(parent)
{
    setFlag(ItemIsSelectable);
    setFlag(ItemIsMovable);
    setAcceptHoverEvents(true);
    isSwitch=false;
    this->mypoint= QPointF();
    mypoint = eventPos;
    nodeID =  itemID;
    rect = boundingRect();
}
QRectF detNode::boundingRect() const {
    return QRectF(mypoint.x()-8,mypoint.y()-7,30,30);
   // qDebug() << mypoint;
}



void detNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {


    if(isSelected()){
        if(isNode){
            QBrush brush(Qt::cyan);
            QPen pen(Qt::black, 3);
            painter->setPen(pen);
            painter->setBrush(brush);
            painter->drawRect(rect.adjusted(0,0,-15,-15));
            painter->drawText(mypoint.x()-8,mypoint.y()-8, QString::number(nodeID)+"_Node");
            update();
        }
        else{
            QBrush brush(Qt::cyan);
            QPen pen(Qt::black, 3);
            painter->setPen(pen);
            painter->setBrush(brush);
            painter->drawEllipse(rect.adjusted(0,0,-15,-15));
            painter->drawText(mypoint.x()-8,mypoint.y()-8, QString::number(nodeID));
            update();
        }
     }

    else if(isNode){
        QBrush brush(Qt::green);
        QPen pen(Qt::black, 3);
        painter->setPen(pen);
        painter->setBrush(brush);
        painter->drawRect(rect.adjusted(0,0,-15,-15));
        painter->drawText(mypoint.x()-8,mypoint.y()-8, QString::number(nodeID)+"_Node");
        update();
    }
   else{
    QBrush brush(Qt::blue);
    QPen pen(Qt::black, 3);
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawEllipse(rect.adjusted(0,0,-15,-15));
    painter->drawText(mypoint.x()-8,mypoint.y()-8, QString::number(nodeID));
    update();
    }
}

void ::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
  rect=rect.adjusted(0,0,5,5);
         update();
}

void switchNode::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
     rect=rect.adjusted(0,0,-5,-5);
     update();
}

void switchNode::mousePressEvent(QGraphicsSceneMouseEvent *event){

    QGraphicsItem::mousePressEvent(event);
}
