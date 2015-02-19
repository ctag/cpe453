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
        bool inDrawing;
        QVector<QLine> *lines;
public:
        void  mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);

        //void  drawLines(QPainter *p);
        void  paintEvent(QPaintEvent *event);


};

#endif // MAINWINDOW_H
