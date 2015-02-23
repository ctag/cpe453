#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
        startPos = QPoint();
        endPos = QPoint();
        leftDown = false;
        rightDown = false;
        connectsToPrevious = false;
        setCentralWidget(ui->centralWidget);
        setMouseTracking(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

<<<<<<< HEAD









//click
void MainWindow::mousePressEvent(QMouseEvent *event){
    if (event->buttons() & Qt::LeftButton)
=======
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    /*if (event->buttons() & Qt::LeftButton)
>>>>>>> origin/master
    {
        if(!connectsToPrevious){
            QLine line = QLine(startPos, endPos);
            lines.append(line);
            startPos = event->pos();
            endPos = event->pos();
            update();

        }
        leftDown = true;
    }

<<<<<<< HEAD
    else if (event->buttons() & Qt::RightButton){
        rightDown = true;
=======
        inDrawing = !inDrawing;
    }*/
    if (event->buttons() & Qt::LeftButton)
    {
        inDrawing = true;
        startPos = event->pos();
>>>>>>> origin/master
    }
}


//click & drag
void MainWindow::mouseMoveEvent(QMouseEvent *event){
    if (leftDown & connectsToPrevious)
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
    else if (connectsToPrevious)
    {
        endPos = event->pos();
        update();
    }
    else if (leftDown){
        startPos = event->pos();
        endPos = event->pos();
        update();
    }
}


//release
void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    if (leftDown){
        leftDown = !leftDown;
        endPos = event->pos();
        QLine line = QLine(startPos, event->pos());
        lines.append(line);

        //begin next line
        startPos = event->pos();
        endPos = event->pos();
        connectsToPrevious = true;

    }
    else if(rightDown){
        //clear selection
        rightDown = !rightDown;
        endPos = startPos;
        connectsToPrevious = false;
    }
    update();
}















void MainWindow::drawLines(QPainter *p)
{
    if (!startPos.isNull() && !endPos.isNull())
    {
        p->drawLine(startPos, endPos);
    }
    p->setRenderHint(QPainter::Antialiasing, true);
    p->drawLines(lines);
}
void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(4);
    p.setPen(pen);
    drawLines(&p);
    p.end();
}

/*
void MainWindow::createNode(double xPos, double yPos){
    //node->QCircle.setScale(0f);
    QPainter p(this);
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(4);
    p.setPen(pen);
    drawLines(&p);
    QRect nodeRec = QRect(xPos, yPos, 12, 12);
}
*/

