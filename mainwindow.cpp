#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QTimer>
#include "abstractserial.h"


MainWindow::MainWindow(QIODevice *ioDev, QWidget *parent) :
        QMainWindow(parent),
        ioDev(ioDev),
        ui(new Ui::MainWindow)
{
    this->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    ui->setupUi(this);
    connect(ioDev, SIGNAL(readyRead()), this, SLOT(onReadyRead()));

    //edited
    AbstractSerial *tempDev = (AbstractSerial *) ioDev;
    tempDev->setDtr(false);
    tempDev->setRts(true);

    save="";
    readFormatedTimer = new QTimer(this);
    readFormatedTimer->start(1000);
    connect(readFormatedTimer,SIGNAL(timeout()),this,SLOT(checkData()));
    ///

    readWaitTimer = new QTimer(this);
    connect(readWaitTimer, SIGNAL(timeout()), this, SLOT(onReadyRead()));

    onReadyRead();
}

///edited
void MainWindow::checkData()
{
    int index=save.indexOf("YY");
    QString print=save.mid(index+2,3);
    qDebug() <<print;
    save="";
}
///

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::printChat(ChatterType type, QString data) {
    data = data.trimmed();
    if(!data.length()) return;
    char colors[][10] = { "blue", "red", "green" };
    char chatterName[][10] = {"Recieve", "System", "Send"};
    ui->chatBoxTextEdit->append(
            QString("<font color='%1'>[%2.%5] %3 : </font>%4\n")
            .arg(colors[type], QTime::currentTime().toString(), chatterName[type], data)
                .arg(QTime::currentTime().msec())
            );
}

void MainWindow::processData(QString data) {
    static QString buffer = "";
    buffer += data;
    if(data.isEmpty()) {
        printChat(Recieve, buffer);
        readWaitTimer->stop();
        buffer = "";
    } else {
        readWaitTimer->start(50);
    }
}


void MainWindow::onReadyRead() {
    static QByteArray buffer = "";
    QByteArray b = ioDev->readAll();
    save+=b;
    buffer += b;
    if(b.length()) QTimer::singleShot(500, this, SLOT(onReadyRead()));
    else {
        processData(buffer);
        buffer = "";
    }
}

void MainWindow::sendTextboxData() {
    QString data = ui->chatlineEdit->text()+"\n";
    sendData(data);
}
//edited
void MainWindow::sendData(QString data) {
    AbstractSerial *tempDev = (AbstractSerial *) ioDev;
    tempDev->setDtr(true);
    tempDev->setRts(false);
    if(ioDev->write(data.toAscii()) == -1) {
         statusBar()->showMessage(tr("Can't send data!"));
         printChat(System, "Can't send data :o");
     } else {
         printChat(Send, data);
     }
    tempDev->setDtr(false);
    tempDev->setRts(true);
}

void MainWindow::on_disconPushButton_clicked()
{
    ioDev->close();
}

void MainWindow::on_sendPushButton_clicked()
{
    sendTextboxData();
}
