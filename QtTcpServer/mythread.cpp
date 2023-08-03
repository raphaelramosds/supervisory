#include "mythread.h"
#include <vector>

using namespace std;

MyThread::MyThread(int ID, QObject *parent, DataStorage* storage) :
    QThread (parent)
{
    this->socketDescriptor = ID;
    this->storage = storage;
}

void MyThread::run(){

    // Thread starts here
    str = QString("<i>") +
          QString().setNum(socketDescriptor) +
          " <font color=\"red\">connected! starting thread</blue></i>";
    emit message(str);

    // Open a socket for the client
    socket = new QTcpSocket();

    // Validate the socket ID
    if(!socket->setSocketDescriptor(this->socketDescriptor)){
        emit error(socket->error());
        return;
    }

    // Connection and disconnection
    connect(
        socket,
        SIGNAL(readyRead()),
        this,
        SLOT(readyRead()),
        Qt::DirectConnection);

    connect(
        socket,
        SIGNAL(disconnected()),
        this,
        SLOT(disconnected()),
        Qt::DirectConnection);

    // Attest the client connection
    str = QString().setNum(socketDescriptor) + " client connected";
    emit message(str);

    peerAddress = socket->peerAddress();

    // Execute the thread
    exec();
}

void MyThread::readyRead(){

    vector<Entry> entry;
    vector<QHostAddress> hostList;

    Entry value;
    qint64 msecdate;
    unsigned int nsamples;

    // Read what user's typed
    QString str = socket->readLine().replace("\n","").replace("\r","");
    emit message(str);

    // Split it into an array
    QStringList list;
    list = str.split(" ");

    // Read the token (get, list, set)
    QString cmd;
    cmd = list.at(0);

    // List producers
    if(cmd == "list"){
        hostList = storage->getHostList();
        for(int i = 0; i < hostList.size(); i++){
            socket->write(hostList[i].toString().toStdString().c_str());
            socket->write("\r\n");
        }
    }

    // Get data produced by a host
    else if(cmd == "get"){

        // Read get arguments
        // e.g. get 127.0.0.1 1

        if(list.size() == 3){

            cmd = list.at(1);
            str = list.at(2);
            nsamples = str.toUInt();

            QHostAddress address(cmd);

            // Validate address
            if(!address.isNull()){

                // Fetch entries for this address
                entry = storage->getData(address, nsamples);

                // Display entries for the client
                for(int i = 0; i < entry.size(); i++){
                    value = entry[i];
                    socket->write(QString().number(value.theTime).toStdString().c_str());
                    socket->write(" ");
                    socket->write(QString().number(value.measurement).toStdString().c_str());
                    socket->write("\n");
                }
            }
        }
    }

    // Inserts telemetry data
    else if(cmd == "set"){

        // Sintaxe: set time_in_ms float_value
        // e.g. set 1496156112708 9.16666

        // time_in_ms : is the diference between the
        // current time and 1970-01-01T00:00:00.000

        if(list.size() == 3){

            bool ok;

            // Get the time
            cmd = list.at(1);

            // Assert if time is a long long
            msecdate = cmd.toLongLong(&ok);

            if(ok){

                // Get the measurement
                cmd = list.at(2);

                // Assert if measurement is a float
                value.measurement = cmd.toFloat(&ok);

                if(ok){
                    // Store data
                    storage->addData(socket->peerAddress(), msecdate, value.measurement);
                }
            }
        }
    }

}

void MyThread::disconnected(){

    // Attest thread's disconnection
    str = QString("<i>") + QString().setNum(socketDescriptor) + " <font color=\"red\">Disconnected</font></i>";
    emit message(str);

    // Close the socket
    socket->deleteLater();

    // Stop the thread
    exit(0);
}
