#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QPainter>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsSceneHoverEvent>
#include <QDebug>
#include <QObject>
#include <QDateTime>
#include <QString>

#include "text.h"
#include "vertex.h"

 class edge : public QGraphicsLineItem
{

public:
     using QGraphicsLineItem::QGraphicsLineItem; // We're using c++11 standards to get around some horrible code styling.
     void setDS(QString _ds);
     void setVertTo(vertex* _vert);
     void setVertFrom(vertex* _vert);
     void setType(QString _type);
     QString getDS();
     vertex* getVertFrom();
     vertex* getVertTo();
     QString getType();


protected:

private:
QString ds;
vertex * vert_from;
vertex * vert_to;
QString type;

 };

#endif // EDGE_H
