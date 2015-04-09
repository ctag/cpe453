#include "vertex.h"

vertex::vertex(QPointF eventPos, int itemID)
{
    doDebug = true; // Set this to enable/disable debugging messages.
    dateTime = QDateTime::currentDateTime();
    setFlag(ItemIsSelectable);
    setFlag(ItemIsMovable);
    setAcceptHoverEvents(true);
    set_node();
    mypoint = eventPos;
    nodeID =  itemID;
    rect = boundingRect();
    draw_track=false;
    nodePosition=eventPos;
    penSize = 2; // 2px pen size
    margin = 3; // 3px between obj and text
    QPointF _textPos(boundingRect().x(), boundingRect().y());
    label = new text(_textPos, QString::number(itemID));
    debugMsg("Loaded new vertex.");
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

void vertex::set_switch()
{
    type="switch";
}

void vertex::set_node()
{
    type="node";
}

QString vertex::get_type()
{
    return(type);
}

bool vertex::is_switch()
{
    return(type=="switch");
}

bool vertex::is_node()
{
    return(type=="node");
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
    int width = 12 + (2*margin);
    int height = 12 + (2*margin);
    return QRectF(mypoint.x()-(width/2),mypoint.y()-(height/2),width,height);
}

/*
 * Total space available in bounding box to draw object
 */
QRectF vertex::drawRect()
{
    return QRectF(boundingRect().adjusted(margin, margin, -(margin), -margin));
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
    if (is_node()) {
        if (brush == NULL) {
            brush = new QBrush(Qt::yellow);
        }
        painter->setBrush(*brush);
        painter->drawEllipse(drawRect());
        //painter->drawText(textRect(), QString::number(nodeID)+"_Node");
    } else if (is_switch()) {
        if (brush == NULL) {
            brush = new QBrush(Qt::green);
        }
        painter->setBrush(*brush);
        painter->drawRect(drawRect());
        //painter->drawText(textRect(), QString::number(nodeID)+"_Switch");
    } else {
        if (brush == NULL) {
            brush = new QBrush(Qt::blue);
        }
        painter->setBrush(*brush);
        painter->drawEllipse(drawRect());
       // painter->drawText(textRect(), QString::number(nodeID));
        label->setText(QString::number(nodeID));
        //label.paint();
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

text* vertex::get_labelPtr()
{
    return(label);
}

void vertex::set_labelLocation()
{
    //QPointF _textPos(boundingRect().x(), boundingRect().y());
    //qDebug() << "Moving label to " << _textPos.x() << _textPos.y();
    label->update_pos(scenePos());
}
















