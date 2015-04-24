#include "track.h"

track::track(QWidget *parent): QGraphicsView(parent)
{
    doDebug = true; // Set this to turn on and off debug messages.
    dateTime = QDateTime::currentDateTime(); // Used for post-analysis.

    this->setEnabled(true);

    leftDown = false;
    rightDown = false;
    dragSelect = false;

    connectsToPrevious = false;
    track_rad_state=false;
    scene = new QGraphicsScene(0,0,300,300);
    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
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

    if(event->buttons()&Qt::LeftButton){
        leftDown = true;
        QGraphicsItem *item;
        item = itemAt(event->pos()); //Check if there is no item at the position
        if (!item){
            dragSelect = true;
        }
    }
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

void track::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    if (leftDown == true)
    {
        leftDown = false;
        dragSelect = false;
    }
}

void track::mouseMoveEvent(QMouseEvent *event) {
    QGraphicsView::mouseMoveEvent(event);

    if(leftDown && dragSelect){
        QGraphicsItem *item;
        item = itemAt(event->pos()); //Get the item at the position
        if (item) //If there is an item at that position
        {
            item->setSelected(true);
        }
    }

    count_edges();
    endPos=mapToScene(event->pos());
    if(!vertexList.isEmpty() && vertexList.contains(dynamic_cast<vertex *>(itemAt(event->pos())))) {
        QList<vertex*> _selected = get_selectedVerts();

        for (int _index = 0; _index < _selected.count(); ++_index)
        {
            vertex* _vert = _selected.at(_index);
            QList<edge*> connected = get_connectedEdges(_vert);

            qDebug() << "Moved: " << _vert->pos() << _vert->mypoint << _vert->nodePosition;
            _vert->set_labelLocation();
            for (int _edgeID = 0; _edgeID < connected.count(); ++_edgeID)
            {
                edge * _edge = connected.at(_edgeID);
                qDebug() << "Edges: " << _edge->pos();
                if ((20 > abs(_edge->line().p1().x() - _vert->nodePosition.x())) && (20 > abs(_edge->line().p1().y() - _vert->nodePosition.y()))) {
                    QLineF _line = _edge->line();
                    _line.setP1(_vert->mypoint + _vert->pos());
                    connected.at(_edgeID)->setLine(_line);
                } else if ((20 > abs(_edge->line().p2().x() - _vert->nodePosition.x())) && (20 > abs(_edge->line().p2().y() - _vert->nodePosition.y()))) {
                    QLineF _line = _edge->line();
                    _line.setP2(_vert->mypoint + _vert->pos());
                    _edge->setLine(_line);
                }
            }
            _vert->nodePosition = _vert->mypoint + _vert->pos();
            //
        }
    }
    //get_connectedEdges();
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
    if(_selected.size()==2)
    {
        // create generic edge
        line= new edge(_selected.at(0)->nodePosition.x(),_selected.at(0)->nodePosition.y(),_selected.at(1)->nodePosition.x(),_selected.at(1)->nodePosition.y());
        line->setType("track");
        line->setVertFrom(_selected.at(0));
        line->setVertTo(_selected.at(1));
        line->setPen(QPen(Qt::black,2,Qt::SolidLine));
        line->setFlag(edge::ItemIsSelectable);
        line->setFlag(edge::ItemIsMovable);

    if ( _selected.at(0)->is_node() && _selected.at(1)->is_node() && _selected.at(0)->edgecount< 2 && _selected.at(1)->edgecount<2 ){
         _selected.at(0)->root=line;
         _selected.at(1)->root=line;
     }
   else if  ((_selected.at(0)->is_switch() &&_selected.at(1)->is_node() && _selected.at(0)->edgecount< 3&&_selected.at(1)->edgecount<2)||(_selected.at(1)->is_switch() &&_selected.at(0)->is_node() && _selected.at(1)->edgecount< 3 &&_selected.at(0)->edgecount<2) )
    {
      if(_selected.at(0)->is_switch() && _selected.at(0)->root==NULL){
        line->setPen(QPen(Qt::black,2,Qt::SolidLine));
        line->setType("root");
        _selected.at(0)->root=line;
        }
      else if(_selected.at(1)->is_switch() && _selected.at(1)->root==NULL){
        line->setPen(QPen(Qt::black,2,Qt::SolidLine));
        line->setType("root");
        _selected.at(1)->root=line;
        }
       else if(_selected.at(0)->is_switch() &&_selected.at(0)->primaryline==NULL && _selected.at(0)->root!=NULL  ){
          line->setPen(QPen(Qt::black,2,Qt::DashLine));
          line->setType("passthrough");

          _selected.at(0)->primaryline=line;
            }
        else if(_selected.at(1)->is_switch() &&_selected.at(1)->primaryline==NULL  && _selected.at(1)->root!=NULL){
          line->setPen(QPen(Qt::black,2,Qt::DashLine));
          line->setType("passthrough");
          _selected.at(1)->primaryline=line;
            }
        else if(_selected.at(0)->is_switch() &&_selected.at(0)->primaryline!=NULL && _selected.at(0)->altline==NULL && _selected.at(0)->root!=NULL){
          line->setPen(QPen(Qt::black,2,Qt::DotLine));
          line->setType("bypass");
          _selected.at(0)->altline=line;
            }
        else if(_selected.at(1)->is_switch() &&_selected.at(1)->primaryline!=NULL &&_selected.at(1)->altline==NULL && _selected.at(1)->root!=NULL){
          line->setPen(QPen(Qt::black,2,Qt::DotLine));
          line->setType("bypass");
          _selected.at(1)->altline=line;
            }
        }
    edgeList.append(line);
    scene->addItem(line);
    }
     update();
}

void track::group_button_clicked(QString _ds){
    do_assignDS(_ds, get_selectedVerts(), get_selectedEdges());
}

void track::delete_button_clicked()
{
    deleteSelected();
}

void track::keyPressEvent(QKeyEvent *event)
{
    QGraphicsView::keyPressEvent(event);

    if(event->key() == Qt::Key_Delete){
        if(!vertexList.isEmpty()){
            deleteSelected();
        }
    }
    else if(event->key() == Qt::Key_Return){
        connect_button_clicked();
    }
    else if(event->key() == Qt::Key_C){
        connect_button_clicked();
    }
    else if(event->key() == Qt::Key_G){
        //addSelectedToNewDS();
    }
    else if(event->key() == Qt::Key_S){
        switch_button_clicked();
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
    QList<edge*> _edgeselected = get_selectedEdges();

    if (!_edgeselected.isEmpty()){
        for (int _index = 0; _index < _edgeselected.size(); _index++){
            edge * _edge = _edgeselected.at(_index);
            if(_edge->pen().style()==Qt::DashLine)
                for(int i=0;i < vertexList.count();i++)
                    if(vertexList.at(i)->collidesWithItem(_edge))
                        vertexList.at(i)->primaryline=NULL;

            if(_edge->pen().style()==Qt::DotLine)
                for(int i=0;i < vertexList.count();i++)
                    if(vertexList.at(i)->collidesWithItem(_edge))
                        vertexList.at(i)->altline=NULL;

            if(_edge->pen().style()==Qt::SolidLine){
                for(int i=0;i < vertexList.count();i++){
                    if(vertexList.at(i)->collidesWithItem(_edge)){
                        vertexList.at(i)->root=NULL;
                    }
                }
            }


            //removeItemFromDetectionSections(_edge);
            scene->removeItem(_edge);
            edgeList.removeOne(_edge);
            delete _edge;
            _index--;
            _edgeselected = get_selectedEdges();
        }
    }
    qDebug() << "successfully deleted edges";

    if (!_vertexselected.isEmpty())
    {
        for (int _index = 0; _index < _vertexselected.size(); _index++)
        {
            vertex * _vert = _vertexselected.at(_index);
            if(!edgeList.empty()){
                for(int j=0; j < edgeList.count();j++){
                    if(_vert->collidesWithItem(edgeList.at(j))){
                        if(edgeList.at(j)->pen().style()==Qt::DashLine)
                                    vertexList.at(_index)->primaryline=NULL;
                        if(edgeList.at(j)->pen().style()==Qt::DotLine)
                                    vertexList.at(_index)->altline=NULL;
                        if(edgeList.at(j)->pen().style()==Qt::SolidLine)
                                    qDebug() << "made it to debug point a";
                                    vertexList.at(_index)->root=NULL; // <-- causing crash when deleting using Ctrl+A selection
                                    qDebug() << "made it to debug point b";
                        //removeItemFromDetectionSections(edgeList[j]);
                        scene->removeItem(edgeList.at(j));
                        edgeList.removeAt(j);
                        qDebug() << "j:" << j << "edgeList.count():" << edgeList.count();
                        j--;

                    }
                }
            }
            //removeItemFromDetectionSections(_vert);
            scene->removeItem(_vert);
            scene->removeItem(_vert->get_labelPtr());
            vertexList.removeOne(_vert);
            delete _vert;
        }
    }
    qDebug() << "successfully deleted verts";

    count_edges();
    //cleanUpDetectionSections(true);
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

QList<edge*> track::get_selectedEdges()
{
    QList<edge*> _selected;
    for (int _index = 0; _index < edgeList.count(); ++_index)
    {
        edge* _edge = edgeList.at(_index);
        if (_edge->isSelected())
        {
            _selected.append(_edge);
        }
    }
    return(_selected);
}



 void track::count_edges(){
    if(!vertexList.isEmpty() && !edgeList.isEmpty()){
        for(int i=0; i < vertexList.count();i++){
            vertexList.at(i)->edgecount=0;
                for(int j=0; j < edgeList.count(); j++){
                    if(vertexList.at(i)->collidesWithItem(edgeList.at(j))){
                        vertexList.at(i)->edgecount++;
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

QList<edge*> track::get_connectedEdges(vertex * _vert)
{
    QList<edge*> connected;
    for(int j=0; j < edgeList.count(); j++){
        if(_vert->collidesWithItem(edgeList.at(j))){
            connected.append(edgeList.at(j));
        }
    }
    return(connected);
}

void track::do_assignDS(QString _ds, QList<vertex *> _verts, QList<edge *> _edges)
{
    for (int _index = 1; _index < _verts.length(); ++_index)
    {
        vertex * _thisVert = _verts.at(_index);
        QList<edge*> _connectedEdges = get_connectedEdges(_thisVert);
        for (int _edgeIndex = 0; _edgeIndex < _connectedEdges.length(); ++_edgeIndex)
        {
            if (_connectedEdges.at(_edgeIndex)->collidesWithItem(_verts.at(_index-1)))
            {
                _edges.append(_connectedEdges.at(_edgeIndex));
            }
        }
    }

    for (int _index = 0; _index < _edges.length(); ++_index)
    {
        _edges.at(_index)->setDS(_ds);
        _edges.at(_index)->setToolTip(_ds);
    }
}

QList<edge*> track::get_allEdges()
{
    emit allEdges(edgeList);
    return(edgeList);
}

void track::updateVertInches()
{
    double minx = 0;
    double miny = 0;
    for (int _index = 0; _index < vertexList.length(); ++_index)
    {
        double x = vertexList.at(_index)->mypoint.x();
        double y = vertexList.at(_index)->mypoint.y();
        if (x < minx)
        {
            minx = x;
        }
        if (y < miny)
        {
            miny = y;
        }
    }
    minx = abs(minx);
    miny = abs(miny);
    for (int _index = 0; _index < vertexList.length(); ++_index)
    {
        vertexList.at(_index)->xInch = (vertexList.at(_index)->mypoint.x() + minx)/10;
        vertexList.at(_index)->yInch = (vertexList.at(_index)->mypoint.y() + miny)/10;
        vertexList.at(_index)->setToolTip(QString::number(vertexList.at(_index)->xInch) + "x" + QString::number(vertexList.at(_index)->yInch));
    }
}

















