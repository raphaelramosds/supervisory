#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QDebug>
#include <QStringList>

#include "mythread.h"
#include "datastorage.h"

/**
 * @brief The MyServer class starts a TCP server
 * able to listenning at 1234 port
 */
class MyServer : public QTcpServer
{
    Q_OBJECT

public:
    /**
    * @brief MyServer is the class constructor
    * @param parent
    */
    MyServer(QObject *parent = 0);

    /**
    * @brief StartServer start the TCP server
    */
    void startServer();

    /**
    * @brief getIPList returns a list of IPs used by server
    * @return
    */
    QStringList getIPList();

signals:
    void message(QString);

public slots:
    void receiveMsg(QString str);

protected:
    /**
    * @brief incomingConnection handles client connections
    * @param socketDescriptor is the socket identifier
    */
    void incomingConnection(qintptr socketDescriptor);

private:
    int ok;
    /**
    * @brief storage handles data storage
    * for different connections
    */
    DataStorage storage;
    QStringList iplist;
};

#endif // MYSERVER_H
