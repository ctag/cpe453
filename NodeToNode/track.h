#ifndef TRACK_H
#define TRACK_H

#include <QLabel>
#include <QEvent>
#include <QMoveEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QGraphicsLineItem>
#include <QPoint>
#include <QGraphicsView>
#include <QPainter>
#include <QDebug>
#include <QDialog>
#include <QLine>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QList>
#include <QDateTime>
#include <QShortcut>
#include <QVector>

#include "vertex.h"

class track : public QGraphicsView
{
    Q_OBJECT

public:
    explicit track(QWidget *parent = 0);
    ~track();
    vertex *previousVertex;
    int activeItemID;
    QGraphicsItem* selectedVertex;
    int id_counter;
    QGraphicsScene* scene;
    bool track_rad_state;
    QPointF startPos;
    QPointF endPos;
    QList<vertex*> vertexList;
    QList<QGraphicsLineItem*> edgeList;
    QList<vertex*> get_selectedVerts();
    QList<QGraphicsLineItem*> get_selectedEdges();
    QList<QGraphicsLineItem*> get_connectedEdges(vertex * _vert);
    QGraphicsLineItem * line;
    void deleteSelected();
    void draw_grid();
    void get_connectedEdges();
    void count_edges();
    QList<QList<QGraphicsItem*> > detectionSections; //--------------------------------------------------
    bool dragSelect;
    int dsCounter; //------------------------------------------------------------------------------------


 private:
    bool firstclick;
    bool secondclick;
    bool originclick;
    bool leftDown;
    bool rightDown;
    bool connectsToPrevious;
    bool doDebug;
    QDateTime dateTime;
    void reSortItemsInDetectionSections(); //-----------------------------------------
    void removeItemFromDetectionSections(QGraphicsItem* itemToRemove); //----------------------------------
    void cleanUpDetectionSections(bool deleteSelected); //-------------------------------------------------

    void listContentsOfAllDetectionSections(); //for debugging only ---------------------------------------

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void debugMsg(QString _msg);
    int shiftAmount; // shift verts num pixels
    int grid_px; // w/h of grid spacing

public slots:
    void get_track_rad(bool status);
    void delete_button_clicked();
    void switch_button_clicked();
    void group_button_clicked();
    void connect_button_clicked();
    void select_all();
    void shift_left();
    void shift_down();
    void shift_up();
    void shift_right();
    QList<vertex*> get_vertices();
    QList<vertex*> get_connectedVertices(vertex*);
    void get_allConnectedVertices();
    void addSelectedToNewDS(); //----------------------------------------------------

signals:
    void vertices(QList<vertex*>);
    void connectedVertices(vertex*, QList<vertex*>);
 //   void positionChange(QPointF );
};

#endif // TRACK_H
