#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <QListWidgetItem>

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
    void listProducers();
    void setProducer(QListWidgetItem *item);
    void getProducerData();
    void tcpConnect();
    void tcpDisconnect();
    void activateTimer();
    void deactivateTimer();

signals:
    void hostConnected(bool);

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QString selectedProducer;
    int timerId;
};

#endif // MAINWINDOW_H
