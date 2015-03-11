#include "mywidget.h"
//#include "mainwindow.h"

mywidget::mywidget(QWidget *parent):QLabel(parent)
{
    this->setMouseTracking(true);
    this->startPos = QPoint();
    this->endPos = QPoint();
    leftDown = false;
    rightDown = false;
    connectsToPrevious = false;
    originclick=true;
    firstclick=false;
    secondclick=false;

}

mywidget::~mywidget(){

}

//click
void mywidget::mousePressEvent(QMouseEvent *event){

    if(track_rad_state==true){
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
    if(switch_rad_state==true)
    {
        if (event->buttons() & Qt::LeftButton)
        {
            if(!connectsToPrevious){
                       QLine line = QLine(startPos, endPos);
                       lines.append(line);
                       startPos = event->pos();
                       endPos = event->pos();
                      update();
                      originclick=true;

                     }

                 leftDown = true;
              }

             else if (event->buttons() & Qt::RightButton){
                rightDown = true;
             }

    }



}


//click & drag
void mywidget::mouseMoveEvent(QMouseEvent *event){

    if(track_rad_state==true){
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

    if(switch_rad_state==true)
    {
         if (connectsToPrevious)
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


}

//release
void mywidget::mouseReleaseEvent(QMouseEvent *event){
   if(track_rad_state==true){

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
   if(switch_rad_state==true){
       if (leftDown){
           leftDown = !leftDown;
           endPos = event->pos();
           QLine line = QLine(startPos, event->pos());
           lines.append(line);

           //begin next line
          if(originclick==true)
         {
              qDebug() <<"Fumborigin";
          endPos = event->pos();
          connectsToPrevious = true;
          originclick=false;
          firstclick=true;
          }
          else if (firstclick)
          {
            qDebug() << "first";
            endPos = event->pos();
            firstclick=false;
            secondclick=true;
             update();

          }
          else if (secondclick)
          {  qDebug() << "second";
            secondclick=false;
            endPos = event->pos();
             update();
             connectsToPrevious=false;
          }


       }
       else if(rightDown){
           //clear selection
           rightDown = !rightDown;
           endPos = startPos;
           if(firstclick)
           connectsToPrevious = false;
           else if (secondclick)
              connectsToPrevious=true;
       }
       update();
   }
}

void mywidget::drawLines(QPainter *p){

   if(track_rad_state==true||switch_rad_state==true){
    if (!startPos.isNull() && !endPos.isNull())
    { p->setRenderHint(QPainter::Antialiasing, true);//added
      p->drawLine(startPos, endPos);
    }

    p->drawLines(lines);
    }

}
void mywidget::paintEvent(QPaintEvent *event){  

     QPainter p(this);

    QPen pen;
    pen.setColor(Qt::black);

    pen.setWidth(3);
    p.setPen(pen);
    drawLines(&p);

}

void mywidget::get_rad_rad(bool status){
 track_rad_state=status;
 detection_rad_state=false;
 switch_rad_state=false;

}

void mywidget::get_det_rad(bool status){
 detection_rad_state=status;
 track_rad_state=false;
 switch_rad_state=false;

}

void mywidget::get_switch_rad(bool status){
    detection_rad_state=false;
    track_rad_state=false;
    switch_rad_state=true;
}

