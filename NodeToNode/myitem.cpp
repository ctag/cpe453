#include <QDebug>
#include "myitem.h"
 #include "mywidget.h"
myitem::myitem(QPointF eventPos, int itemID)
{
     setFlag(ItemIsSelectable);
    setFlag(ItemIsMovable);
    setAcceptHoverEvents(true);
    isNode=false;
    isSwitch=false;
     // this->mypoint= QPointF();
    mypoint = eventPos;
    nodeID =  itemID;
    rect = boundingRect();
    draw_track=false;
  }
QRectF myitem::boundingRect() const {
    return QRectF(mypoint.x()-8,mypoint.y()-7,30,30);
   // qDebug() << mypoint;
}



void myitem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {


    if(isSelected()){
        if(isSwitch){
            QBrush brush(Qt::cyan);
            QPen pen(Qt::black, 3);
            painter->setPen(pen);
            painter->setBrush(brush);
            painter->drawRect(rect.adjusted(0,0,-15,-15));
            painter->drawText(mypoint.x()-8,mypoint.y()-8, QString::number(nodeID)+"_Switch");
            update();
        }
        else if (isNode){
            QBrush brush(Qt::cyan);
            QPen pen(Qt::black, 3);
            painter->setPen(pen);
            painter->setBrush(brush);
            painter->drawEllipse(rect.adjusted(0,0,-15,-15));
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
        QBrush brush(Qt::yellow);
        QPen pen(Qt::black,3);
        painter->setPen(pen);
        painter->setBrush(brush);
        painter->drawEllipse(rect.adjusted(0,0,-15,-15));
        painter->drawText(mypoint.x()-8,mypoint.y()-8, QString::number(nodeID)+"_Node");
        update();
    }
    else if(isSwitch){
        QBrush brush(Qt::green);
        QPen pen(Qt::black,3);
        painter->setPen(pen);
        painter->setBrush(brush);
        painter->drawRect(rect.adjusted(0,0,-15,-15));
        painter->drawText(mypoint.x()-8,mypoint.y()-8, QString::number(nodeID)+"_Switch");
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

void myitem::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
     rect=rect.adjusted(0,0,5,5);
     update();
  }

void myitem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
     rect=rect.adjusted(0,0,-5,-5);
     update();
}



