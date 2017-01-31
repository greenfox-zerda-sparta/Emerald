#include "broadcastsocket.h"

BroadcastSocket::BroadcastSocket(QObject *parent) : QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    isRunning = false;
    startUDP();
}

void BroadcastSocket::startUDP()
{
    isRunning = true;
    udpSocket->bind(45454, QUdpSocket::ShareAddress);
    connect(udpSocket, SIGNAL(readyRead()),
         this, SLOT(processPendingDatagrams()));
}

void BroadcastSocket::processPendingDatagrams()
 {
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        if (isRunning)
        {
            emit newDatagram("c:connect");
            isRunning = false;
        }
    }
 }

void BroadcastSocket::close()
{
	udpSocket->close();
}
