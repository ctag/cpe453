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
    shiftAmount = 40;

    // Protect undeclared pointers.
    previousVertex = NULL;
    selectedVertex = NULL;
    line = NULL;

    // Allow selecting all verts with ctrl+a
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_A), this, SLOT(select_all()));

    // Hotkeys for moving all verts
    new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Down), this, SLOT(shift_down()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Left), this, SLOT(shift_left()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Up), this, SLOT(shift_up()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Right), this, SLOT(shift_right()));

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
    QGraphicsView::mousePressEvent(event);
    QPointF p = mapToScene(event->pos());

    if(!itemAt(event->pos()) && (event->buttons()&Qt::RightButton)){
         vertex * _vert = new vertex(p,id_counter);

         startPos = _vert->pos();
         scene->addItem(_vert);
         scene->addItem(_vert->get_labelPtr());
         id_counter++;
         vertexList.append(_vert);
         update();
    }
    else if(track_rad_state && event->buttons() & Qt::RightButton && vertexList.contains(dynamic_cast<vertex *>(itemAt(event->pos())))){
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

        vertex * _vert = new vertex(endPos,id_counter);
        id_counter++;
        vertexList.append(_vert);
    }
}

void track::mouseMoveEvent(QMouseEvent *event) {
    QGraphicsView::mouseMoveEvent(event);
    endPos=mapToScene(event->pos());
    if(!vertexList.isEmpty() && vertexList.contains(dynamic_cast<vertex *>(itemAt(event->pos())))) {
        QList<vertex*> _selected = get_selectedVerts();
        for (int _index = 0; _index < _selected.count(); ++_index)
        {
            vertex* _vert = _selected.at(_index);
            _vert->set_labelLocation();
        }
        /*vertex * _vert=dynamic_cast<vertex *>(itemAt(event->pos()));
         if (event->buttons() & Qt::LeftButton)
         {
             _vert->set_labelLocation();
         }
        if(_vert->isSelected() && _vert->ItemPositionChange) {
            _vert->nodePosition=endPos;
        }*/
    }
}

void track::switch_button_clicked() {
    QList<vertex*> _selected = get_selectedVerts();
    if (!_selected.isEmpty())
    {
        for (int _index = 0; _index < _selected.size(); ++_index)
        {
            vertex * _vert = _selected.at(_index);
            _vert->set_switch();
        }
    }
    update();
}

void track::node_button_clicked(){
    QList<vertex*> _selected = get_selectedVerts();
    if (!_selected.isEmpty())
    {
        for (int _index = 0; _index < _selected.size(); ++_index)
        {
            vertex * _vert = _selected.at(_index);
            _vert->set_node();
        }
    }
    update();
}

void track::delete_button_clicked()
{
    QList<vertex*> _selected = get_selectedVerts();
    if (!_selected.isEmpty())
    {
        for (int _index = 0; _index < _selected.size(); ++_index)
        {
            vertex * _vert = _selected.at(_index);
            scene->removeItem(_vert);
            vertexList.removeOne(_vert);
        }
    }
    update();
}

void track::keyPressEvent(QKeyEvent *event)
{
    QGraphicsView::keyPressEvent(event);
    if (vertexList.isEmpty())
    {
        return;
    }
    if(event->key() == Qt::Key_Delete){
        vertex * _vert;
       for(int i=0;i<vertexList.size();i++){
          if(vertexList.at(i)->isSelected()){
             _vert=vertexList.at(i);
             scene->removeItem(_vert);
             vertexList.removeAt(i);
              i--;
           update();
          }

       }
   }
}

void track::select_all()
{
    for (int _index = 0; _index < vertexList.count(); ++_index)
    {
        vertex * _vert = vertexList.at(_index);
        _vert->setSelected(true);
    }
    update();
}

void track::shift_left()
{
    qDebug() << "Shifting left.";
    for (int _index = 0; _index < vertexList.count(); ++_index)
    {
        vertex * _vert = vertexList.at(_index);
        _vert->moveBy(-shiftAmount, 0);
        _vert->set_labelLocation();
    }
}

void track::shift_down()
{
    for (int _index = 0; _index < vertexList.count(); ++_index)
    {
        vertex * _vert = vertexList.at(_index);
        _vert->moveBy(0, shiftAmount);
        _vert->set_labelLocation();
    }
}
void track::shift_up()
{
    for (int _index = 0; _index < vertexList.count(); ++_index)
    {
        vertex * _vert = vertexList.at(_index);
        _vert->moveBy(0, -shiftAmount);
        _vert->set_labelLocation();
    }
}
void track::shift_right()
{
    for (int _index = 0; _index < vertexList.count(); ++_index)
    {
        vertex * _vert = vertexList.at(_index);
        _vert->moveBy(shiftAmount, 0);
        _vert->set_labelLocation();
    }
}

void track::get_track_rad(bool status)
{
    track_rad_state=status;
}

QList<vertex*> track::get_selectedVerts()
{
    QList<vertex*> _selected;
    for (int _index = 0; _index < vertexList.count(); ++_index)
    {
        vertex * _vert = vertexList.at(_index);
        if (_vert->isSelected())
        {
            _selected.append(_vert);
        }
    }
    return(_selected);
}
















