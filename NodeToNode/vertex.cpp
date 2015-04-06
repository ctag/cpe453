#include "vertex.h"

vertex::vertex(QPointF eventPos, int itemID)
{
    doDebug = true; // Set this to enable/disable debugging messages.
    dateTime = QDateTime::currentDateTime();

    setFlag(ItemIsSelectable);
    setFlag(ItemIsMovable);
    setAcceptHoverEvents(true);
    isNode=false;
    isSwitch=false;
    //this->mypoint= QPointF();
    mypoint = eventPos;
    nodeID =  itemID;
    rect = boundingRect();
    draw_track=false;

    debugMsg("Loaded myitem type.");
}

vertex::~vertex()
{
    //
}

void vertex::debugMsg(QString _msg)
{
    if (doDebug)
    {
        qDebug() << dateTime.toString("[HH:mm:ss:zzz] ") + _msg;
    }
}

QRectF vertex::boundingRect() const
{
    return QRectF(mypoint.x()-8,mypoint.y()-7,30,30);
}

void vertex::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
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

void vertex::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
     rect=rect.adjusted(0,0,5,5);
     update();
}

void vertex::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
     rect=rect.adjusted(0,0,-5,-5);
     update();
}



