#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <QListWidgetItem>

namespace Ui {
class MainWindow;
}

struct Entry {
    qlonglong time;
    QString measurement;
};

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
    void tcpConnect();
    void tcpDisconnect();
    void activateTimer();
    void deactivateTimer();

signals:
    void hostConnected(bool);
    void entryChanged(Entry*);
    void timerActivated(int);
    void timerDeactivated();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QString selectedProducer;
    int timerId;
};

#endif // MAINWINDOW_H
