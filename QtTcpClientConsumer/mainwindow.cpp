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

    // List producers on listWidget
    connect(
        this,
        SIGNAL(hostConnected(bool)),
        this,
        SLOT(listProducers())
    );

    // Set producer whose data will be displayed
    connect(
        ui->listWidgetProducers,
        SIGNAL(itemClicked(QListWidgetItem*)),
        this,
        SLOT(setProducer(QListWidgetItem*))
    );

    // Start and stop getting producer's data
    connect(
        ui->pushButtonStart,
        SIGNAL(clicked(bool)),
        this,
        SLOT(activateTimer())
    );

    connect(
        ui->pushButtonStop,
        SIGNAL(clicked(bool)),
        this,
        SLOT(deactivateTimer())
    );

    // Set timing value
    connect(
        ui->horizontalSliderTiming,
        SIGNAL(valueChanged(int)),
        ui->labelTimingValue,
        SLOT(setNum(int)));
}


MainWindow::~MainWindow()
{
    delete socket;
    delete ui;
}

void MainWindow::tcpConnect(){

    socket->connectToHost(ui->lineEditAddress->text(),1234);

    if(socket->waitForConnected(3000)){
        qDebug() << "Connected";
        emit hostConnected(true);
    } else{
        qDebug() << "Disconnected";
    }
}

void MainWindow::tcpDisconnect()
{
    qDebug() << "Disconnecting";
    socket->disconnectFromHost();
}

void MainWindow::activateTimer() {
    QString timing = ui->labelTimingValue->text();
    timerId = startTimer(timing.toInt() * 1000);
}

void MainWindow::deactivateTimer() {
    killTimer(timerId);
}

void MainWindow::getProducerData(){
    QString str;
    QStringList list;
    qint64 thetime;

    if(socket->state() == QAbstractSocket::ConnectedState){
        if(socket->isOpen()){

            QString request = QString("get ") + selectedProducer + QString(" 5\r\n");

            socket->write(request.toStdString().c_str());
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

void MainWindow::timerEvent(QTimerEvent *event) {
    getProducerData();
}

void MainWindow::listProducers()
{
    QString str;

    if(socket->state() == QAbstractSocket::ConnectedState){

        // Fetch all producers
        socket->write("list");
        socket->waitForBytesWritten();
        socket->waitForReadyRead();

        // Add them on the list widget
        while(socket->bytesAvailable()){
            str = socket->readLine().replace("\n","").replace("\r","");
            ui->listWidgetProducers->addItem(str);
        }
    }
}


void MainWindow::setProducer(QListWidgetItem *item)
{
    selectedProducer = item->text();
}
