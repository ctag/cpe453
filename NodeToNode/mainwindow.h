#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLine>
#include <QEvent>
#include <QMoveEvent>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QPoint>
#include <QPixmap>
#include <QPainter>
#include <QDebug>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QAction>
#include <QRadioButton>
#include <QThread>

#include "sql.h"
#include "vertex.h"

 namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QPoint mappedPos;

private:
    Ui::MainWindow *ui;
    vertex *activeNode;
    QGraphicsScene* scene;
    QList<vertex> nodeList;
    bool connectsToPrevious;
    SQL * locosql;
    QThread threadSQL;

 private slots:
    // det_node_pressed();
    //void switch_pressed();
    void check_rad();
    void handle_DBopened();
    void handle_DBclosed();
    void do_connectDB();
    void do_disconnectDB();

 signals:
        void rad_track_status(bool status);
        void rad_det_status(bool status);
        void rad_switch_status(bool status);
        void rad_grab_status(bool status);

};

#endif // MAINWINDOW_H
