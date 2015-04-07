#include "vertex.h"

/*
 * Bounding format:
 * Entire Object:
 * x: 30 + 2xpen + 2px
 * y: 40 + 2xpen + 2px
 * Text: 30px (+ 30px) by 10px
 * Painting: 30px by 30px
 */

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
    penSize = 2; // 2 px pen size
    margin = 3;
    textWidth = 30;

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

/**
 * @brief vertex::boundingRect
 * @return
 * This is the Absolute total space we may draw in.
 * MUST be larger than
 */
QRectF vertex::boundingRect() const
{
    //return QRectF(mypoint.x()-8,mypoint.y()-7,30,30);
    int width = 30 + (penSize*2) + 2 + textWidth;
    int height = 40 + (penSize*2) + 2 + margin;
    return QRectF(mypoint.x()-(width/2),mypoint.y()-(height/2),width,height);
}

/*
 * Total space available in bounding box to draw object
 */
QRectF vertex::drawRect()
{
    int padding = penSize + 1;
    return QRectF(boundingRect().adjusted(padding, 10+padding+margin, -(padding+textWidth), -padding));
}

/*
 * Total space available in bounding box to draw object
 */
QRectF vertex::textRect()
{
    int padding = penSize + 1;
    return QRectF(boundingRect().adjusted(padding, padding, (textWidth-padding), -(30+padding)));
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
        painter->drawEllipse(drawRect());
        painter->drawText(textRect(), QString::number(nodeID)+"_Node");
    } else if (isSwitch) {
        if (brush == NULL) {
            brush = new QBrush(Qt::green);
        }
        painter->setBrush(*brush);
        painter->drawRect(drawRect());
        painter->drawText(textRect(), QString::number(nodeID)+"_Switch");
    } else {
        if (brush == NULL) {
            brush = new QBrush(Qt::blue);
        }
        painter->setBrush(*brush);
        painter->drawEllipse(drawRect());
        painter->drawText(textRect(), QString::number(nodeID));
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



