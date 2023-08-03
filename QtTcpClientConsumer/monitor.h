#ifndef MONITOR_H
#define MONITOR_H

#include <QWidget>

class Monitor : public QWidget
{
    Q_OBJECT

public:
    explicit Monitor(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);

signals:

};

#endif // MONITOR_H
