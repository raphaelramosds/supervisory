#include "myserver.h"
#include <QNetworkInterface>
MyServer::MyServer(QObject *parent) :
    QTcpServer(parent)
{

}

void MyServer::startServer(){
    if(!this->listen(QHostAddress::Any, 1234)){
        emit message(QString("<b>Server did not start!</b>"));
    }
    else{
        emit message(QString("<b>Server started!</b>"));

        // Loop over available addreses for the server
        foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {

            // Register all server addresses
            if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
                iplist << address.toString();
        }
    }
}

QStringList MyServer::getIPList(){
    return iplist;
}

void MyServer::receiveMsg(QString str){
    emit message(str);
}

void MyServer::incomingConnection(qintptr socketDescriptor){

    // Display the client's socket ID
    QString str;
    str = QString("<i>") + QString().setNum(socketDescriptor) + " connecting...</i>";
    emit message(str);

    // Open a thread for handling client's requests
    MyThread *thread = new MyThread(socketDescriptor,this, &storage);

    // Thread's object will be deleted once the thread is finished
    connect(
        thread,
        SIGNAL(finished()),
        thread,
        SLOT(deleteLater()));

    // Thread will emmit messages that will be emmitted by the server
    connect(
        thread,
        SIGNAL(message(QString)),
        this,
        SLOT(receiveMsg(QString)));

    // Run the thread
    thread->run();
}
