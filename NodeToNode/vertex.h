#ifndef VERTEX_H
#define VERTEX_H

#include <QGraphicsItem>
#include <QPainter>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsSceneHoverEvent>
#include <QDebug>
#include <QObject>
#include <QDateTime>

#include "text.h"

 class vertex : public QGraphicsItem
{

public:
    vertex(QPointF eventPos, int itemID);
    ~vertex();
    QPointF mypoint;
    int xposition;
    int yposition;
    QRectF boundingRect() const;
    QRectF drawRect();
    QRectF textRect();
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
    void set_switch();
    void set_node();
    QString get_type();
    bool is_switch();
    bool is_node();
    bool draw_track;
    int vertexID;
    int penSize; // width of pen brush
    int margin; // distance between text and painting
    int textWidth; // Extra space for text
    QRectF rect;
    text* get_labelPtr();
    void set_labelLocation();

    QPointF nodePosition;


protected:

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void debugMsg(QString _msg);
    QString type;

private:
    bool doDebug;
    QDateTime dateTime;
    text * label;

public slots:

signals:

 };

#endif // VERTEX_H
