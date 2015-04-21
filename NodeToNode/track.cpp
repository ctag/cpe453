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
    grid_px = 20;

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

    draw_grid();

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

void track::draw_grid()
{
    int width = 1000;
    int height = 1000;
    qDebug() << width << height;
    QPen pen(QColor(150, 255, 150, 150), 1);
    for (int _index = -100; _index < 100; ++_index)
    {
        scene->addLine((_index*grid_px), -height, (_index*grid_px), height, pen);
        scene->addLine(-width, (_index*grid_px), width, (_index*grid_px), pen);
    }
}

void track::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    QPointF p = mapToScene(event->pos());

    if(!vertexList.contains(dynamic_cast<vertex *>(itemAt(event->pos()))) && (event->buttons()&Qt::RightButton)){
         vertex * _vert = new vertex(p,id_counter);

         startPos = _vert->pos();
         scene->addItem(_vert);
         _vert->setZValue(1);
         scene->addItem(_vert->get_labelPtr());
         id_counter++;
         vertexList.append(_vert);
         update();
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
           _selected.at(_index)->nodePosition= _selected.at(_index)->mypoint+_selected.at(_index)->pos();
         }

    }
    get_connectedEdges();
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

void track::connect_button_clicked() {
    QList<vertex*> _selected = get_selectedVerts();
    if (_selected.size()==2 &&  _selected.at(0)->is_node() && _selected.at(0)->count_edge<2)
    {
        line= new QGraphicsLineItem(_selected.at(0)->nodePosition.x(),_selected.at(0)->nodePosition.y(),_selected.at(1)->nodePosition.x(),_selected.at(1)->nodePosition.y());
        line->setPen(QPen(Qt::black,2));
        line->setFlag(QGraphicsLineItem::ItemIsSelectable);
        line->setFlag(QGraphicsLineItem::ItemIsMovable);
        edgeList.append(line);
         scene->addItem(line);
         get_selectedEdges();

    }
    else if (_selected.size()==2 &&  _selected.at(0)->is_switch() && _selected.at(0)->count_edge < 3)
    {
        if(_selected.at(0)->count_edge ==1){
        line= new QGraphicsLineItem(_selected.at(0)->nodePosition.x(),_selected.at(0)->nodePosition.y(),_selected.at(1)->nodePosition.x(),_selected.at(1)->nodePosition.y());
        line->setPen(QPen(Qt::black,2,Qt::DashDotLine));
        line->setFlag(QGraphicsLineItem::ItemIsSelectable);
        line->setFlag(QGraphicsLineItem::ItemIsMovable);
        edgeList.append(line);
        scene->addItem(line);
        get_selectedEdges();}
        else if(_selected.at(0)->count_edge ==2){
            line= new QGraphicsLineItem(_selected.at(0)->nodePosition.x(),_selected.at(0)->nodePosition.y(),_selected.at(1)->nodePosition.x(),_selected.at(1)->nodePosition.y());
            line->setPen(QPen(Qt::black,2,Qt::DashDotDotLine));
            line->setFlag(QGraphicsLineItem::ItemIsSelectable);
            line->setFlag(QGraphicsLineItem::ItemIsMovable);
            edgeList.append(line);
            scene->addItem(line);
            get_selectedEdges();}
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
    deleteSelected();
}

void track::keyPressEvent(QKeyEvent *event)
{
    QGraphicsView::keyPressEvent(event);
    if (vertexList.isEmpty())
    {
        return;
    }
    if(event->key() == Qt::Key_Delete){
        deleteSelected();
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
    for (int _index = 0; _index < vertexList.count(); ++_index)
    {
        vertex * _vert = vertexList.at(_index);
        _vert->moveBy(-shiftAmount, 0);
        _vert->set_labelLocation();
    }
    for (int _index = 0; _index < edgeList.count(); ++_index)
    {
        line = edgeList.at(_index);
        line->moveBy(-shiftAmount, 0);
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
    for (int _index = 0; _index < edgeList.count(); ++_index)
    {
        line = edgeList.at(_index);
        line->moveBy(0, shiftAmount);
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
    for (int _index = 0; _index < edgeList.count(); ++_index)
    {
        line = edgeList.at(_index);
        line->moveBy(0, -shiftAmount);
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
    for (int _index = 0; _index < edgeList.count(); ++_index)
    {
        line = edgeList.at(_index);
        line->moveBy(shiftAmount, 0);
    }
}

void track::get_track_rad(bool status)
{
    track_rad_state=status;
}

void track::deleteSelected()
{
    QList<vertex*> _vertexselected = get_selectedVerts();
    QList<QGraphicsLineItem*> _edgeselected = get_selectedEdges();

    if (!_vertexselected.isEmpty())
    {
        for (int _index = 0; _index < _vertexselected.size(); ++_index)
        {
            vertex * _vert = _vertexselected.at(_index);
            if(!edgeList.empty()){
                for(int j=0; j < edgeList.count();j++){
                    if(_vert->collidesWithItem(edgeList.at(j))){
                        scene->removeItem(edgeList.at(j));
                        edgeList.removeAt(j);
                    }
                }
            }
            scene->removeItem(_vert);
            scene->removeItem(_vert->get_labelPtr());
            vertexList.removeOne(_vert);
            delete _vert;
        }
    }

    if (!_edgeselected.isEmpty())
    {
        for (int _index = 0; _index < _edgeselected.size(); ++_index)
        {
            QGraphicsLineItem * _edge = _edgeselected.at(_index);
            scene->removeItem(_edge);
            edgeList.removeOne(_edge);
            delete _edge;
        }
    }
    get_connectedEdges();
    update();


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

QList<QGraphicsLineItem*> track::get_selectedEdges()
{
    QList<QGraphicsLineItem*> _selected;
    for (int _index = 0; _index < edgeList.count(); ++_index)
    {
        QGraphicsLineItem* _edge = edgeList.at(_index);
        if (_edge->isSelected())
        {
            _selected.append(_edge);
        }
    }
    return(_selected);
}



void track::get_connectedEdges(){

    if(!vertexList.isEmpty() && !edgeList.isEmpty()){
        for(int i=0; i < vertexList.count();i++){
            vertexList.at(i)->count_edge=0;
                 for(int j=0; j < edgeList.count(); j++){
                    if(vertexList.at(i)->collidesWithItem(edgeList.at(j))){
                        vertexList.at(i)->count_edge++;
                    }
                }
        }
     }
}


QList<vertex*> track::get_vertices()
{
    return(vertexList);
}

QList<vertex*> track::get_connectedVertices(vertex * _vertex)
{
QList<vertex*> connected;
if(!vertexList.isEmpty() && !edgeList.isEmpty()){ // sanity check
    for(int j=0; j < edgeList.count(); j++){
        if(_vertex->collidesWithItem(edgeList.at(j))){
            for(int k=0; k < vertexList.count();k++){
                if(_vertex!= vertexList.at(k)){
                    if(edgeList.at(j)->collidesWithItem(vertexList.at(k))){
                        connected.append(vertexList.at(k));
                    }
                }
            }
        }
    }
}
return(connected);
}

void track::get_allConnectedVertices()
{
if(!vertexList.isEmpty() && !edgeList.isEmpty()){
    for(int i=0; i < vertexList.count();i++){
        QList<vertex*> connected;
        for(int j=0; j < edgeList.count(); j++){
            if(vertexList.at(i)->collidesWithItem(edgeList.at(j))){
                    for(int k=0; k < vertexList.count();k++){
                        if(vertexList.at(i)!= vertexList.at(k)){
                            if(edgeList.at(j)->collidesWithItem(vertexList.at(k))){
                             connected.append(vertexList.at(k));
                        }
                    }
                }
            }
        }
        emit connectedVertices(vertexList.at(i), connected);
    }
}
}









