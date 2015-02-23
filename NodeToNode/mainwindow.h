#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QLine"
#include "QEvent"
#include "QMoveEvent"
#include "QGraphicsSceneMouseEvent"
#include <QMouseEvent>
#include <QPoint>
#include <QPixmap>
#include <QPainter>
#include <QDebug>
#include <QRect>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void drawLines(QPainter *p);

        QPoint startPos;
        QPoint endPos;
<<<<<<< HEAD
        bool leftDown;
        bool rightDown;
        bool connectsToPrevious;
        QVector<QLine> lines;
=======
        bool inDrawing;
        QVector<QLine> *lines;
>>>>>>> origin/master
public:
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);

        //void  drawLines(QPainter *p);
        void paintEvent(QPaintEvent *paintEvent);


};

#endif // MAINWINDOW_H
