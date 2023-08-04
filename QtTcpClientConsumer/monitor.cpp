#include "monitor.h"
#include <QPainter>
#include <QBrush>
#include <QPen>

Monitor::Monitor(QWidget *parent)
    : QWidget{parent}
{
    // First entry is zero
    entries.push_back({.time=0,.measurement=0});
}

void Monitor::paintEvent(QPaintEvent *event)
{
    qlonglong t1, t2;
    int m1, m2, i;

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

    // Set lines color
    pen.setColor(QColor(0,0,255));
    pen.setWidth(2);
    painter.setPen(pen);

    std::function<int(Entry)> f_measure = [&](Entry it) {
        return height() - it.measurement.toInt()*height()/100;
    };

    // Plot Time versus Measurement
    i = 0;
    t1 = 0;
    m1 = f_measure(entries.at(0));

    for (auto it : entries) {
        t2 = width()*i/30;
        m2 = f_measure(it);
        painter.drawLine(t1,m1,t2,m2);
        t1 = t2;
        m1 = m2;
        i += 1;
    }

}

void Monitor::timerEvent(QTimerEvent *event)
{
    repaint();
}

void Monitor::setEntry(Entry* entry)
{
    if (entries.size() >= 30) {
        // Delete first element
        entries.erase(entries.begin());
    }
    entries.push_back(*entry);
}

void Monitor::turnOnMonitor(int timing)
{
    timerId = startTimer(timing);
}

void Monitor::turnOffMonitor()
{
    killTimer(timerId);
}

