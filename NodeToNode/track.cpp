#include "track.h"

track::track(QWidget *parent): QGraphicsView(parent)
{
    doDebug = true; // Set this to turn on and off debug messages.
    dateTime = QDateTime::currentDateTime(); // Used for post-analysis.

    this->setEnabled(true);

    leftDown = false;
    rightDown = false;

    connectsToPrevious = false;
    track_rad_state=false;
    scene = new QGraphicsScene(0,0,300,300);
    this->setScene(scene);
    id_counter=0;

    // Protect undeclared pointers.
    activeNode = NULL;
    previousNode = NULL;
    selectedNode = NULL;
    testline = NULL;
    line = NULL;

    debugMsg("Loaded graphics widget.");
 }

track::~track()
{
    //
}

void track::debugMsg(QString _msg)
{
    if (doDebug)
    {
        qDebug() << dateTime.toString("[HH:mm:ss:zzz] ") + _msg;
    }
}

//click
void track::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    QPointF p = mapToScene(event->pos());

    if(event->buttons() & Qt::RightButton){
         activeNode = new vertex(p,id_counter);

         startPos = activeNode->pos();
         scene->addItem(activeNode);
         id_counter++;

         nodeList.append(activeNode);
         update();
     }
    if( event->buttons() & Qt::LeftButton)
        leftDown=true;

  }

void track::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    if(leftDown)
        leftDown=!leftDown;
}

void track::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
    if(itemAt(event->pos())){
         activeNode=dynamic_cast<vertex *>(itemAt(event->pos()));
     }
}

void track::switch_button_clicked(){
    if(!nodeList.isEmpty() && activeNode->isSelected())
    {
         activeNode->isSwitch=!activeNode->isSwitch;
          activeNode->isNode=false;
         update();}
}

void track::node_button_clicked(){
    if(!nodeList.isEmpty()  && activeNode->isSelected())
    {
         activeNode->isNode=!activeNode->isNode;
         activeNode->isSwitch=false;
         update();}

}

void track::delete_button_clicked()
{
     if(activeNode->isSelected())
         scene->removeItem(activeNode);
         update();
}

void track::keyPressEvent(QKeyEvent *event)
{
     QGraphicsView::keyPressEvent(event);
    qDebug() << event->key();
    if (!activeNode)
    {
        return;
    }
    if(activeNode->isSelected() && event->key() == Qt::Key_Delete){
        scene->removeItem(activeNode);
        update();
    }
}

void track::get_track_rad(bool status)
{
    track_rad_state=status;
}
