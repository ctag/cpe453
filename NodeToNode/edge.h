#ifndef EDGE_H
#define EDGE_H

#include <QObject>
#include <QGraphicsLineItem>
#include <QPainter>

class edge : public QObject,QGraphicsLineItem
{
    Q_OBJECT
public:
   edge(QPointF beginPos,QPointF finalPos );
   QPointF linestartPos;
   QPointF lineendPos;

signals:

protected:
   void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public slots:
void updatePosition();
};

#endif // EDGE_H
