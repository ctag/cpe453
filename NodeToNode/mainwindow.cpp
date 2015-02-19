#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
        startPos = QPoint();
        endPos = QPoint();
        inDrawing = false;
        setMouseTracking(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    /*if (event->buttons() & Qt::LeftButton)
    {
        if (!inDrawing)
        {
            startPos = event->pos();
        }
        else
        {
            endPos = event->pos();

            QLine line = QLine(startPos, event->pos());
            lines.append(line);
        }

        inDrawing = !inDrawing;
    }*/
    if (event->buttons() & Qt::LeftButton)
    {
        inDrawing = true;
        startPos = event->pos();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (inDrawing)
    {
        endPos = event->pos();
        //update();
        repaint();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    //according to QT help, event->Buttons will not operate on the triggering button in a mouse release event.
    //therefore, QT::LeftButton should be false.
    if (event->buttons() & !Qt::LeftButton)
    {
        inDrawing = false;
        endPos = event->pos();
        QLine line = new QLine(startPos, event->pos());
        lines.append(line);
    }
}

void MainWindow::drawLines(QPainter *p)
{
    if (!startPos.isNull() && !endPos.isNull())
    {
        p->drawLine(startPos, endPos);
    }

    p->drawLines(lines);
}
void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(5);
    p.setPen(pen);
    drawLines(&p);
    p.end();
}
