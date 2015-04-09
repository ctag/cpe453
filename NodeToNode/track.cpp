#include "track.h"

track::track(QWidget *parent): QGraphicsView(parent)
{
    doDebug = true; // Set this to turn on and off debug messages.
    dateTime = QDateTime::currentDateTime(); // Used for post-analysis.

    this->setEnabled(true);

    leftDown = false;
    rightDown = false;

    connectsToPrevious = false;
    track_rad_state=false; // What the fuck is this?
    scene = new QGraphicsScene(0,0,300,300);
    this->setScene(scene);
    id_counter=0;

    // Protect undeclared pointers.
    activeVertex = NULL;
    previousNode = NULL;
    selectedNode = NULL;
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

void track::mousePressEvent(QMouseEvent *event)
{
    qDebug() << itemAt(event->pos());

    QGraphicsView::mousePressEvent(event);
    QPointF p = mapToScene(event->pos());

    if(!itemAt(event->pos()) && (event->buttons()&Qt::RightButton)){
         activeVertex = new vertex(p,id_counter);

         startPos = activeVertex->pos();
         scene->addItem(activeVertex);
         id_counter++;

         nodeList.append(activeVertex);
         update();
     }
    else if(track_rad_state && event->buttons() & Qt::RightButton && nodeList.contains(dynamic_cast<vertex *>(itemAt(event->pos())))){
        rightDown=true;
        startPos=mapToScene(event->pos());
        line = new edge(startPos,endPos);
        scene->addItem(line);
        update();
    }


  }

void track::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    if(rightDown && !itemAt(event->pos())){
        rightDown=!rightDown;
        line=new QGraphicsLineItem(startPos.x(),startPos.y(),endPos.x(),endPos.y());
        line->setPen(QPen(Qt::black,5));
        line->setFlag(QGraphicsLineItem::ItemIsSelectable);
        line->setFlag(QGraphicsLineItem::ItemIsMovable);
        scene->addItem(line);

        activeVertex= new vertex(endPos,id_counter);
        id_counter++;
        nodeList.append(activeVertex);
    }
}

void track::mouseMoveEvent(QMouseEvent *event){
    QGraphicsView::mouseMoveEvent(event);
    endPos=mapToScene(event->pos());
    if(!nodeList.isEmpty() && nodeList.contains(dynamic_cast<vertex *>(itemAt(event->pos())))){
         activeVertex=dynamic_cast<vertex *>(itemAt(event->pos()));
        if(activeVertex->isSelected() && activeVertex->ItemPositionChange){
            activeVertex->nodePosition=endPos;
        }
    }
}

void track::switch_button_clicked(){
    if(!nodeList.isEmpty() && activeVertex->isSelected())
    {
         activeVertex->set_switch();
         update();}
}

void track::node_button_clicked(){
    if(!nodeList.isEmpty()  && activeVertex->isSelected())
    {
        for(int i=0;i<nodeList.size();i++){
           if(nodeList.at(i)->isSelected()){
              activeVertex=nodeList.at(i);
              activeVertex->set_node();
            update();
           }
        }

}}

void track::delete_button_clicked()
{
    if(!nodeList.isEmpty()){
        for(int i=0;i<nodeList.size();i++){
           if(nodeList.at(i)->isSelected()){
              activeVertex=nodeList.at(i);
              scene->removeItem(activeVertex);
              nodeList.removeAt(i);
               i--;
            update();
           }
        }
    }
}

void track::keyPressEvent(QKeyEvent *event)
{
    QGraphicsView::keyPressEvent(event);
    if(!nodeList.isEmpty() && event->key() == Qt::Key_Delete){
       for(int i=0;i<nodeList.size();i++){
          if(nodeList.at(i)->isSelected()){
             activeVertex=nodeList.at(i);
             scene->removeItem(activeVertex);
             nodeList.removeAt(i);
              i--;
           update();
          }

       }
   }
}

void track::get_track_rad(bool status)
{
    track_rad_state=status;
}
