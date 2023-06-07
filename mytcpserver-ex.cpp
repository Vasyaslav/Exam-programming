#include "mytcpserver-ex.h"
#include "funcs.cpp"
#include <QDebug>
#include <QCoreApplication>
#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QVector>

MyTcpServer::~MyTcpServer()
{
    //mTcpSocket->close();
    mTcpServer->close();
    server_status=0;
}
MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent){
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, &QTcpServer::newConnection,
            this, &MyTcpServer::slotNewConnection);

    if(!mTcpServer->listen(QHostAddress::Any, 33333)){
        qDebug() << "server is not started";
    } else {
        server_status=1;
        qDebug() << "server is started";
    }
}

void MyTcpServer::slotNewConnection(){
    QTcpSocket* socket = mTcpServer->nextPendingConnection();
    if(server_status==1){
        socket->write("Сервер запущен!\r\n");
        connect(socket, &QTcpSocket::readyRead,
                this,&MyTcpServer::slotServerRead);
        connect(socket,&QTcpSocket::disconnected,
                this,&MyTcpServer::slotClientDisconnected);
    }
    mTcpSocket.insert(socket->socketDescriptor(), socket);
}

void MyTcpServer::slotServerRead(){
    QTcpSocket* socket = (QTcpSocket*)sender();
    QString req;
    while(socket->bytesAvailable()>0)
    {
        req.append(socket->readLine());
    }
    if (req.size() > 0 and req.back() == '\n'){
        socket->write(step_find(req.split("\r\n")[0]).toUtf8());
    }
    req.clear();
}

void MyTcpServer::slotClientDisconnected(){
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->close();
}

QString MyTcpServer::step_find(QString data){
    //qDebug() << data;
    QList data_split = data.split('&');
    QString res = "Результат:";
    QVector<double> numbers;
    if (data_split[0] == "sort"){
        //qDebug() << data_split[2];
        //qDebug() << data_split[2].split(' ').size();
        for (int i = 0; i < data_split[2].split(' ').size(); i++)
            numbers.append(data_split[2].split(' ')[i].toDouble());
        heap_sort(numbers, numbers.size(), data_split[1].toInt());
        for (int i = 0; i < data_split[2].split(' ').size(); i++)
            res = res + " " + QString::number(numbers[i]);
        //qDebug() << res;
        //sort&3&1 5 3 4 2
        //sort&5&12 5 6 9 33 79 2 5
    }
    res = res + ".\n";
    return res;
}
