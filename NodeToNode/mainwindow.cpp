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
    if (event->buttons() & Qt::LeftButton)
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
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (inDrawing)
    {
        endPos = event->pos();
        update();
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
}
