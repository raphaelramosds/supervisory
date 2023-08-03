#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create socket
    socket = new QTcpSocket(this);

    // Connect and disconnect TCP
    connect(
        ui->pushButtonConnect,
        SIGNAL(clicked(bool)),
        this,
        SLOT(tcpConnect()));

    connect(
        ui->pushButtonDisconnect,
        SIGNAL(clicked(bool)),
        this,
        SLOT(tcpDisconnect()));

    /*
    connect(ui->pushButtonGet,
            SIGNAL(clicked(bool)),
            this,
            SLOT(getData()));
    */
}

void MainWindow::tcpConnect(){

    socket->connectToHost(ui->lineEditAddress->text(),1234);

    if(socket->waitForConnected(3000)){
        qDebug() << "Connected";
    } else{
        qDebug() << "Disconnected";
    }
}

void MainWindow::tcpDisconnect()
{
    qDebug() << "Disconnecting";
    socket->disconnectFromHost();
}

void MainWindow::getData(){
    QString str;
    QByteArray array;
    QStringList list;
    qint64 thetime;
    qDebug() << "to get data...";
    if(socket->state() == QAbstractSocket::ConnectedState){
        if(socket->isOpen()){
            qDebug() << "reading...";
            socket->write("get 127.0.0.1 5\r\n");
            socket->waitForBytesWritten();
            socket->waitForReadyRead();
            qDebug() << socket->bytesAvailable();
            while(socket->bytesAvailable()){
                str = socket->readLine().replace("\n","").replace("\r","");
                list = str.split(" ");
                if(list.size() == 2){
                    bool ok;
                    str = list.at(0);
                    thetime = str.toLongLong(&ok);
                    str = list.at(1);
                    qDebug() << thetime << ": " << str;
                }
            }
        }
    }
}


MainWindow::~MainWindow()
{
    delete socket;
    delete ui;
}
