#include "mywidget.h"

mywidget::mywidget(QWidget *parent):QLabel(parent)
{
    this->setMouseTracking(true);
    this->startPos = QPoint();
    this->endPos = QPoint();
    leftDown = false;
    rightDown = false;
    connectsToPrevious = false;

}

mywidget::~mywidget(){

}

//click
void mywidget::mousePressEvent(QMouseEvent *event){

    if (event->buttons() & Qt::LeftButton)
    {
        if(!connectsToPrevious){
                   QLine line = QLine(startPos, endPos);
                   lines.append(line);
                   startPos = event->pos();

           endPos = event->pos();
         update();
        }
     leftDown = true;
     }

    else if (event->buttons() & Qt::RightButton){
            rightDown = true;
        }
    }



//click & drag
void mywidget::mouseMoveEvent(QMouseEvent *event){

    if (leftDown & connectsToPrevious)
    {
        endPos = event->pos();
        update();
    }
    else if (connectsToPrevious)
        {
            endPos = event->pos();
            update();
        }
   else if (leftDown){
            startPos = event->pos();
            endPos = event->pos();
            update();
        }

}

//release
void mywidget::mouseReleaseEvent(QMouseEvent *event){

    if (leftDown){
        leftDown = !leftDown;
        endPos = event->pos();
        QLine line = QLine(startPos, event->pos());
        lines.append(line);

        //begin next line
        startPos = event->pos();
        endPos = event->pos();
        connectsToPrevious = true;

    }
    else if(rightDown){
        //clear selection
        rightDown = !rightDown;
        endPos = startPos;
        connectsToPrevious = false;
    }
    update();

}

void mywidget::drawLines(QPainter *p)
{
    if (!startPos.isNull() && !endPos.isNull())
    { p->setRenderHint(QPainter::Antialiasing, true);//added
      p->drawLine(startPos, endPos);
    }

    p->drawLines(lines);
}
void mywidget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    QPen pen;
    pen.setColor(Qt::black);

    pen.setWidth(5);
    p.setPen(pen);
    drawLines(&p);

}

