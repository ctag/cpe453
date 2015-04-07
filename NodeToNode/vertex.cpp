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
    QBrush * brush = NULL;
    QPen pen(Qt::black, 2);
    painter->setPen(pen);

    if(isSelected())
    {
        brush = new QBrush(Qt::cyan);
    }
    if (isNode) {
        if (brush == NULL) {
            brush = new QBrush(Qt::yellow);
        }
        painter->setBrush(*brush);
        painter->drawEllipse(rect.adjusted(0,0,-15,-15));
        painter->drawText(mypoint.x()-8,mypoint.y()-8, QString::number(nodeID)+"_Node");
    } else if (isSwitch) {
        if (brush == NULL) {
            brush = new QBrush(Qt::green);
        }
        painter->setBrush(*brush);
        painter->drawRect(rect.adjusted(0,0,-15,-15));
        painter->drawText(mypoint.x()-8,mypoint.y()-8, QString::number(nodeID)+"_Switch");
    } else {
        if (brush == NULL) {
            brush = new QBrush(Qt::blue);
        }
        painter->setBrush(*brush);
        painter->drawEllipse(rect.adjusted(0,0,-15,-15));
        painter->drawText(mypoint.x()-8,mypoint.y()-8, QString::number(nodeID));
    }

    update();
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



