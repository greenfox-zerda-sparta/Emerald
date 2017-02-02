#include "broadcastsocket.h"

BroadcastSocket::BroadcastSocket(QObject *parent) : QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    isRunning = false;
    connect(udpSocket, SIGNAL(readyRead()),
         this, SLOT(processPendingDatagrams()));
    startUDP();
}

void BroadcastSocket::startUDP()
{
    isRunning = true;
    bool connState = false;
    do {
        connState = udpSocket->bind(45454, QUdpSocket::ShareAddress);
    } while(!connState);
    QDebug qdebug = qDebug();
    qdebug.noquote() << "   UDP Socket has bound, state: " << udpSocket->state();
}

void BroadcastSocket::processPendingDatagrams()
 {
    if(udpSocket->state() == QUdpSocket::BoundState){
        while (udpSocket->hasPendingDatagrams()) {
            QByteArray datagram;
            datagram.resize(udpSocket->pendingDatagramSize());
            udpSocket->readDatagram(datagram.data(), datagram.size());
            if (isRunning)
            {
                emit newDatagram("/connect");
            }
        }
    }
 }

void BroadcastSocket::close()
{
    if (isRunning)
    {
        udpSocket->close();
    }
    isRunning = false;
}
