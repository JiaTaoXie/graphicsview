#include "viewport.h"
#include <QDebug>
#include <QPainter>

Viewport::Viewport(QWidget* parent, Qt::WindowFlags flags)
    : QOpenGLWidget(parent,flags)
{
    qDebug() << "view port finished";
}

void Viewport::paintEvent(QPaintEvent *e)
{
    QOpenGLWidget::paintEvent(e);
    qDebug() << "Viewport::paintEvent";

//    QPainter painter;
//    painter.begin(this);
//    painter.setPen(QPen(Qt::red,2));
//    painter.drawLine(QPoint(0,0),QPoint(width(),height()));

//    painter.end();
}
