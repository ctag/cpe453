 #ifndef TEXT_H
#define TEXT_H

#include <QGraphicsItem>
#include <QPainter>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsSceneHoverEvent>
#include <QDebug>
#include <QObject>
#include <QDateTime>
#include <QString>

 class text : public QGraphicsItem
{

public:
    text(QPointF _pos, QString _text);
    ~text();
    QPointF mypoint;
    QRectF boundingRect() const;
    QRectF textRect();
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
    int nodeID;
    QRectF rect;
    QString displayText;
    void setText(QString _text);
    void update_pos(QPointF _point);


protected:
    void debugMsg(QString _msg);

private:
    bool doDebug;
    QDateTime dateTime;

public slots:

signals:

 };

#endif // TEXT_H
