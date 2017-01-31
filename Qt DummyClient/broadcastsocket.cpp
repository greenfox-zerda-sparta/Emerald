#include "broadcastsocket.h"

BroadcastSocket::BroadcastSocket(QObject *parent) : QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(45454, QUdpSocket::ShareAddress);

    connect(udpSocket, SIGNAL(readyRead()),
         this, SLOT(processPendingDatagrams()));
    //connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
}

void BroadcastSocket::processPendingDatagrams()
 {
     while (udpSocket->hasPendingDatagrams()) {
         QByteArray datagram;
         datagram.resize(udpSocket->pendingDatagramSize());
         udpSocket->readDatagram(datagram.data(), datagram.size());
         emit newDatagram(QString::fromUtf8(datagram));
     }
 }
