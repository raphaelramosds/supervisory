#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
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
    void timerEvent(QTimerEvent *event);

public slots:
    void tcpConnect();
    void tcpDisconnect();
    void activateTimer();
    void deactivateTimer();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    int timerId;
};

#endif // MAINWINDOW_H
