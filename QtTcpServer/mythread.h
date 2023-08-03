#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QStringList>
#include <QString>
#include <QHostAddress>
#include "datastorage.h"

/**
 * @brief The MyThread creates a thread that
 * handles incoming TCP connections
 */
class MyThread : public QThread {
    Q_OBJECT

public:
    /**
    * @brief MyThread is the class constructor
    * @param ID is the thread identifier
    * @param parent
    * @param storage
    */
    MyThread(int ID, QObject *parent, DataStorage* storage);
    void run();

signals:
    void error(QTcpSocket::SocketError socketerror);
    void message(QString);

public slots:
    /**
     * @brief readyRead is the request handler
     * @details it verifies the token (list, get or set)
     * and perform the service related to it
    */
    void readyRead();
    void disconnected();

private:
    QTcpSocket *socket;
    int socketDescriptor;
    int* ok;
    QHostAddress peerAddress;
    DataStorage *storage;
    QString str;
};

#endif // MYTHREAD_H
