#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){

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

    // Activate and deactivate timer
    connect(
        ui->pushButtonStart,
        SIGNAL(clicked(bool)),
        this,
        SLOT(activateTimer()));

    connect(
        ui->pushButtonStop,
        SIGNAL(clicked(bool)),
        this,
        SLOT(deactivateTimer()));

    // Set timing value
    connect(
        ui->horizontalSliderTiming,
        SIGNAL(valueChanged(int)),
        ui->labelTimingValue,
        SLOT(setNum(int)));
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

    // In case of the stop button was not pressed
    killTimer(timerId);
}

void MainWindow::activateTimer()
{
    // Kill possible previous timer
    killTimer(timerId);

    // Start a new one
    QString timing = ui->labelTimingValue->text();
    timerId = startTimer(timing.toInt() * 1000);
}

void MainWindow::deactivateTimer()
{
    killTimer(timerId);
}


MainWindow::~MainWindow(){
    delete socket;
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    QDateTime datetime;
    QString str;
    qint64 msecdate;
    int min, max, measurement;

    if(socket->state() == QAbstractSocket::ConnectedState){
        min = ui->horizontalSliderMin->value();
        max = ui->horizontalSliderMax->value();
        measurement = rand()%(max - min + 1) + min;
        msecdate = QDateTime::currentDateTime().toMSecsSinceEpoch();

        str = "set "+ QString::number(msecdate) + " " + QString::number(measurement) + "\r\n";

        qDebug() << str;
        qDebug() << socket->write(str.toStdString().c_str()) << " bytes written";

        if(socket->waitForBytesWritten(3000)){
            qDebug() << "wrote";
        }

        ui->textBrowserHistory->append(str);
    }

}

