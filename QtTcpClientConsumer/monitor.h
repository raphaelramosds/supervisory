#ifndef MONITOR_H
#define MONITOR_H

#include "mainwindow.h"
#include <QWidget>
#include <vector>

class Monitor : public QWidget
{
    Q_OBJECT

public:
    explicit Monitor(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);

public slots:
    void setEntry(Entry* entry);
    void turnOnMonitor(int timing);
    void turnOffMonitor();

private:
    std::vector<Entry> entries;
    int timerId;

signals:

};

#endif // MONITOR_H
