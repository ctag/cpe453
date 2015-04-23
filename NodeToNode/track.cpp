#include "track.h"

track::track(QWidget *parent): QGraphicsView(parent)
{
    doDebug = true; // Set this to turn on and off debug messages.
    dateTime = QDateTime::currentDateTime(); // Used for post-analysis.

    this->setEnabled(true);

    leftDown = false;
    rightDown = false;
    dragSelect = false;

    dsCounter = 0; //global int used to keep track of the number of detection sections in the track
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
    if(leftDown = true){
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
            QList<QGraphicsLineItem*> connected = get_connectedEdges(_vert);

            qDebug() << "Moved: " << _vert->pos() << _vert->mypoint << _vert->nodePosition;
            _vert->set_labelLocation();
            for (int _edgeID = 0; _edgeID < connected.count(); ++_edgeID)
            {
                QGraphicsLineItem * _edge = connected.at(_edgeID);
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
    if(_selected.size()==2){
    if ( _selected.at(0)->is_node() && _selected.at(1)->is_node() && _selected.at(0)->edgecount< 2 && _selected.at(1)->edgecount<2 ){
        line= new QGraphicsLineItem(_selected.at(0)->nodePosition.x(),_selected.at(0)->nodePosition.y(),_selected.at(1)->nodePosition.x(),_selected.at(1)->nodePosition.y());
        line->setPen(QPen(Qt::black,2,Qt::SolidLine));
        line->setFlag(QGraphicsLineItem::ItemIsSelectable);
        line->setFlag(QGraphicsLineItem::ItemIsMovable);
        edgeList.append(line);
         scene->addItem(line);
         get_selectedEdges();
         _selected.at(0)->root=line;
         _selected.at(1)->root=line;


     }
   else if  ((_selected.at(0)->is_switch() &&_selected.at(1)->is_node() && _selected.at(0)->edgecount< 3&&_selected.at(1)->edgecount<2)||(_selected.at(1)->is_switch() &&_selected.at(0)->is_node() && _selected.at(1)->edgecount< 3 &&_selected.at(0)->edgecount<2) )
    {
      if(_selected.at(0)->is_switch() && _selected.at(0)->root==NULL){
        line= new QGraphicsLineItem(_selected.at(0)->nodePosition.x(),_selected.at(0)->nodePosition.y(),_selected.at(1)->nodePosition.x(),_selected.at(1)->nodePosition.y());
        line->setPen(QPen(Qt::black,2,Qt::SolidLine));
        line->setFlag(QGraphicsLineItem::ItemIsSelectable);
        line->setFlag(QGraphicsLineItem::ItemIsMovable);
        edgeList.append(line);
        scene->addItem(line);
        _selected.at(0)->root=line;
        }
      else if(_selected.at(1)->is_switch() && _selected.at(1)->root==NULL){
        line= new QGraphicsLineItem(_selected.at(0)->nodePosition.x(),_selected.at(0)->nodePosition.y(),_selected.at(1)->nodePosition.x(),_selected.at(1)->nodePosition.y());
        line->setPen(QPen(Qt::black,2,Qt::SolidLine));
        line->setFlag(QGraphicsLineItem::ItemIsSelectable);
        line->setFlag(QGraphicsLineItem::ItemIsMovable);
        edgeList.append(line);
        scene->addItem(line);
        _selected.at(1)->root=line;
        }
       else if(_selected.at(0)->is_switch() &&_selected.at(0)->primaryline==NULL && _selected.at(0)->root!=NULL  ){
          line= new QGraphicsLineItem(_selected.at(0)->nodePosition.x(),_selected.at(0)->nodePosition.y(),_selected.at(1)->nodePosition.x(),_selected.at(1)->nodePosition.y());
          line->setPen(QPen(Qt::black,2,Qt::DashLine));
          line->setFlag(QGraphicsLineItem::ItemIsSelectable);
          line->setFlag(QGraphicsLineItem::ItemIsMovable);
          edgeList.append(line);
          scene->addItem(line);
          _selected.at(0)->primaryline=line;
            }
        else if(_selected.at(1)->is_switch() &&_selected.at(1)->primaryline==NULL  && _selected.at(1)->root!=NULL){
          line= new QGraphicsLineItem(_selected.at(0)->nodePosition.x(),_selected.at(0)->nodePosition.y(),_selected.at(1)->nodePosition.x(),_selected.at(1)->nodePosition.y());
          line->setPen(QPen(Qt::black,2,Qt::DashLine));
          line->setFlag(QGraphicsLineItem::ItemIsSelectable);
          line->setFlag(QGraphicsLineItem::ItemIsMovable);
          edgeList.append(line);
          scene->addItem(line);
          _selected.at(1)->primaryline=line;
            }
        else if(_selected.at(0)->is_switch() &&_selected.at(0)->primaryline!=NULL && _selected.at(0)->altline==NULL && _selected.at(0)->root!=NULL){
          line= new QGraphicsLineItem(_selected.at(0)->nodePosition.x(),_selected.at(0)->nodePosition.y(),_selected.at(1)->nodePosition.x(),_selected.at(1)->nodePosition.y());
          line->setPen(QPen(Qt::black,2,Qt::DotLine));
          line->setFlag(QGraphicsLineItem::ItemIsSelectable);
          line->setFlag(QGraphicsLineItem::ItemIsMovable);
          edgeList.append(line);
          scene->addItem(line);
          _selected.at(0)->altline=line;
            }
        else if(_selected.at(1)->is_switch() &&_selected.at(1)->primaryline!=NULL &&_selected.at(1)->altline==NULL && _selected.at(1)->root!=NULL){
          line= new QGraphicsLineItem(_selected.at(0)->nodePosition.x(),_selected.at(0)->nodePosition.y(),_selected.at(1)->nodePosition.x(),_selected.at(1)->nodePosition.y());
          line->setPen(QPen(Qt::black,2,Qt::DotLine));
          line->setFlag(QGraphicsLineItem::ItemIsSelectable);
          line->setFlag(QGraphicsLineItem::ItemIsMovable);
          edgeList.append(line);
          scene->addItem(line);
          _selected.at(1)->altline=line;
            }
        }
    }
     update();
}

void track::group_button_clicked(){
    addSelectedToNewDS();
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
        addSelectedToNewDS();
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
    QList<QGraphicsLineItem*> _edgeselected = get_selectedEdges();

    if (!_edgeselected.isEmpty()){
        for (int _index = 0; _index < _edgeselected.size(); _index++){
            QGraphicsLineItem * _edge = _edgeselected.at(_index);
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


            removeItemFromDetectionSections(_edge);
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
                                    qDebug() << "/!\  made it to debug point a";
                                    vertexList.at(_index)->root=NULL; // <-- causing crash when deleting using Ctrl+A selection
                                    qDebug() << "/!\  made it to debug point b";
                        removeItemFromDetectionSections(edgeList[j]);
                        scene->removeItem(edgeList.at(j));
                        edgeList.removeAt(j);
                        qDebug() << "j:" << j << "edgeList.count():" << edgeList.count();
                        j--;

                    }
                }
            }
            removeItemFromDetectionSections(_vert);
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

QList<QGraphicsLineItem*> track::get_connectedEdges(vertex * _vert)
{
    QList<QGraphicsLineItem*> connected;
    for(int j=0; j < edgeList.count(); j++){
        if(_vert->collidesWithItem(edgeList.at(j))){
            connected.append(edgeList.at(j));
        }
    }
    return(connected);
}






//----------------------------------------------------------------------------------------------
// CREATE NEW EMPTY DETECTION SECTION, AND ADD SELECTION TO IT  (CLEANS UP AFTER ITSELF)
//----------------------------------------------------------------------------------------------
void track::addSelectedToNewDS(){
    qDebug() << "---------NEW DETECTION SECTION ADDED----------";
    int addedVertexCount = 0;
    int addedEdgeCount = 0;

    reSortItemsInDetectionSections();
    cleanUpDetectionSections(false);
    dsCounter = detectionSections.count();

    //create an empty new detection section (QList) called _itemList
    QList<QGraphicsItem*> _itemList;
    QList<vertex*> _selectedVerts = get_selectedVerts();
    QList<QGraphicsLineItem*> _selectedEdges = get_selectedEdges();

    //add selected verts to _itemList
    for (int _index = 0; _index < _selectedVerts.count(); _index++){
        vertex * _vert = _selectedVerts.at(_index);
        if (_vert->isSelected()){
            _itemList.append(_vert);
            addedVertexCount++; //for debugging only
        }
    }

    //cycle through all edges, add to _itemGroup
    for (int _index = 0; _index < _selectedEdges.count(); _index++){
        QGraphicsLineItem * _edge = _selectedEdges.at(_index);
        if (_edge->isSelected()){
            _itemList.append(_edge);
            addedEdgeCount++; //for debugging only
        }
    }

    qDebug() << "+ added" << addedVertexCount << "vertices and" << addedEdgeCount << "edges to ds" << dsCounter;
    cleanUpDetectionSections(false);
    detectionSections.append(_itemList);
    listContentsOfAllDetectionSections(); //uneeded, for debugging purposes only
    //dsCounter = detectionSections.count();
}

//---------------------------------------------------------------------------------------------
// CALLED IN addSelectedToNewDS() TO DISASSOCIATE ITEMS FROM ANY PREVIOUS DETECTION SECTIONS
//---------------------------------------------------------------------------------------------
void track::reSortItemsInDetectionSections(){
    //eliminate selected nodes or edges from participating in any other detection sections
    if (!detectionSections.isEmpty()){
        for (int _dsIndex = 0; _dsIndex < detectionSections.count(); _dsIndex++){
            int numberOfItemsMoved = 0;
            if (!detectionSections[_dsIndex].isEmpty()){
                for (int _itemIndex = 0; _itemIndex < detectionSections[_dsIndex].count(); _itemIndex++){
                    QGraphicsItem * _item = detectionSections.at(_dsIndex).at(_itemIndex);
                    if (_item->isSelected()){
                        numberOfItemsMoved++;
                        detectionSections[_dsIndex].removeAt(_itemIndex);
                        _itemIndex--; //repeat at current index, everything just got bumped down

                    }
                }
            }
            if(numberOfItemsMoved > 0){
                qDebug() << numberOfItemsMoved << "item(s) moved from ds " << _dsIndex << "to new ds";

            }
        }
    }
}


//-----------------------------------------------------------------------------------
// CALL BEFORE DELETING ITEMS: REMOVES PASSED QGraphicsItem FROM ITS DETECTION SECTION
//-----------------------------------------------------------------------------------
void track::removeItemFromDetectionSections(QGraphicsItem* itemToRemove){
    for (int _dsIndex = 0; _dsIndex < detectionSections.count(); _dsIndex++){
        if (!detectionSections.isEmpty()){
            for(int _itemIndex = 0; _itemIndex < detectionSections[_dsIndex].count(); _itemIndex++){
                if(itemToRemove == dynamic_cast<vertex *>(detectionSections[_dsIndex][_itemIndex])) {
                    qDebug() << "removed a deleted vertex from it's detection section";
                    detectionSections[_dsIndex].removeAt(_itemIndex);
                    _itemIndex--;
                }
                else if(itemToRemove == dynamic_cast<QGraphicsLineItem *>(detectionSections[_dsIndex][_itemIndex])) {
                    qDebug() << "removed a deleted edge from it's detection section";
                    detectionSections[_dsIndex].removeAt(_itemIndex);
                    _itemIndex--;
                }
            }
        }
    }
}


//-----------------------------------------------------------------------------------
// DELETES EMPTY DETECTION SECTIONS & RESORTS.
//-----------------------------------------------------------------------------------
void track::cleanUpDetectionSections(bool deleteSelected){

    //delete any empty detectionSections
    if (!detectionSections.isEmpty()){
        for (int _dsIndex = 0; _dsIndex < detectionSections.count(); _dsIndex++){
            if(detectionSections[_dsIndex].isEmpty()){
                detectionSections.removeAt(_dsIndex);
                _dsIndex--;
            }
            //else remove selected items from DS if delete key was pressed
            else if (deleteSelected){
                for(int _itemIndex = 0; _itemIndex < detectionSections[_dsIndex].count(); _itemIndex++){
                    if(get_selectedVerts().contains(dynamic_cast<vertex *>(detectionSections[_dsIndex][_itemIndex]))) {
                        qDebug() << "removed a deleted vertex from it's detection section";
                        detectionSections[_dsIndex].removeAt(_itemIndex);
                        _itemIndex--;
                    }
                    else if(get_selectedEdges().contains(dynamic_cast<QGraphicsLineItem *>(detectionSections[_dsIndex][_itemIndex]))) {
                        qDebug() << "removed a deleted edge from it's detection section";
                        detectionSections[_dsIndex].removeAt(_itemIndex);
                        _itemIndex--;
                    }
                }
            }
        }
    }
}

//-------------------------------------------------------------------------------------------
// FOR DEBUGGING ONLY: Lists all the vertices and edges of each detection node in the console
//-------------------------------------------------------------------------------------------
void track::listContentsOfAllDetectionSections(){
    //list contents of detection sections
    qDebug() << "----------------------------------------------";
    if (!detectionSections.isEmpty()){
        for (int _dsIndex = 0; _dsIndex < detectionSections.count(); _dsIndex++){
            if (!detectionSections[_dsIndex].isEmpty()){
                int vCount = 0;
                int eCount = 0;
                for (int _itemIndex = 0; _itemIndex < detectionSections[_dsIndex].count(); _itemIndex++){
                    if(!vertexList.isEmpty() && vertexList.contains(dynamic_cast<vertex *>(detectionSections[_dsIndex][_itemIndex]))) {
                        vCount++;
                    }
                    else{
                        eCount++;
                    }
                }
                qDebug() << "ds" << _dsIndex << " contains" << vCount << "vertices and" << eCount << "edges";
            }
        }
    }
    qDebug() << " ";
    qDebug() << " ";
}








