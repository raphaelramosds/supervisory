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
    brush.setColor(QColor(40,40,40));
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    painter.drawRect(0,0,width(),height());
}
