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

 class vertex : public QObject, public QGraphicsItem
{
    Q_OBJECT
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
    bool isSwitch;
    bool isNode;
    bool draw_track;
    int nodeID;
    int penSize; // width of pen brush
    int margin; // distance between text and painting
    int textWidth; // Extra space for text
    QRectF rect;

    QPointF nodePosition;


protected:

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void debugMsg(QString _msg);

private:
    bool doDebug;
    QDateTime dateTime;

public slots:

signals:

 };

#endif // VERTEX_H
