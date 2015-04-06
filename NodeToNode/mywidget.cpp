#include "mywidget.h"

mywidget::mywidget(QWidget *parent): QGraphicsView(parent)
{
    this->setEnabled(true);

    leftDown = false;
    rightDown = false;

    connectsToPrevious = false;
    track_rad_state=false;
    scene = new QGraphicsScene(0,0,300,300);
    this->setScene(scene);
    id_counter=0;
 }

mywidget::~mywidget(){

}

//click
void mywidget::mousePressEvent(QMouseEvent *event){
    QGraphicsView::mousePressEvent(event);
    QPointF p = mapToScene(event->pos());

    if(event->buttons() & Qt::RightButton){
         activeNode = new myitem(p,id_counter);

         startPos = activeNode->pos();
              scene->addItem(activeNode);
               id_counter++;
         nodeList.append(activeNode);
         update();
     }
    if( event->buttons() & Qt::LeftButton)
        leftDown=true;

  }

void mywidget::mouseReleaseEvent(QMouseEvent *event){
    QGraphicsView::mouseReleaseEvent(event);
    if(leftDown)
        leftDown=!leftDown;
}

void mywidget::mouseMoveEvent(QMouseEvent *event){
    QGraphicsView::mouseMoveEvent(event);
    if(itemAt(event->pos())){
         activeNode=dynamic_cast<myitem *>(itemAt(event->pos()));
     }
}

void mywidget::switch_button_clicked(){
    if(!nodeList.isEmpty() && activeNode->isSelected()){
         activeNode->isSwitch=!activeNode->isSwitch;
          activeNode->isNode=false;
         update();}
}

void mywidget::node_button_clicked(){
    if(!nodeList.isEmpty()  && activeNode->isSelected()){
         activeNode->isNode=!activeNode->isNode;
         activeNode->isSwitch=false;
         update();}

}

void mywidget::delete_button_clicked(){
     if(activeNode->isSelected())
         scene->removeItem(activeNode);
         update();
}

void mywidget::keyPressEvent(QKeyEvent *event){
     QGraphicsView::keyPressEvent(event);
    qDebug() << event->key();
    if(activeNode->isSelected() && event->key() == Qt::Key_Delete){
        scene->removeItem(activeNode);
        update();
    }
}

void mywidget::get_track_rad(bool status){
    track_rad_state=status;
 }
