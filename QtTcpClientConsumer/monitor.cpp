#include "monitor.h"
#include <QPainter>
#include <QBrush>
#include <QPen>

Monitor::Monitor(QWidget *parent)
    : QWidget{parent}
{

}

void Monitor::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QBrush brush;
    QPen pen;

    // Draw the canvas
    brush.setColor(QColor(250,250,250));
    brush.setStyle(Qt::SolidPattern);
    pen.setColor(QColor(250,250,250));
    painter.setBrush(brush);
    painter.setPen(pen);
    painter.drawRect(0,0,width(),height());
}
