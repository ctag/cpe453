#include "text.h"

text::text(QPointF _pos, QString _text)
{
    doDebug = true; // Set this to enable/disable debugging messages.
    dateTime = QDateTime::currentDateTime();
    //setFlag(ItemIsSelectable);
    setFlag(ItemIsMovable);
    mypoint = _pos;
    setText(_text);
    rect = boundingRect();
    debugMsg("Loaded new text.");
}

text::~text()
{
    //
}

void text::debugMsg(QString _msg)
{
    if (doDebug)
    {
        qDebug() << dateTime.toString("[HH:mm:ss:zzz] ") + _msg;
    }
}

/**
 * @brief text::boundingRect
 * @return
 * This is the Absolute total space we may draw in.
 * MUST be larger than
 */
QRectF text::boundingRect() const
{
    return QRectF(mypoint.x(),mypoint.y()-(10),80,10);
}

/*
 * Total space available in bounding box to draw object
 */
QRectF text::textRect()
{
    //return QRectF(boundingRect().adjusted(margin, margin, -(margin), -margin));
    return boundingRect();
}

void text::setText(QString _text)
{
    displayText = _text;
    update();
}

void text::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    qDebug() << "drawing text";
    QBrush * brush = NULL;
    QPen pen(Qt::black, 2);
    painter->setPen(pen);

brush = new QBrush(Qt::yellow);
    painter->setBrush(*brush);
    painter->drawEllipse(textRect());

    //painter->drawText(textRect(), displayText);
    update();
}


