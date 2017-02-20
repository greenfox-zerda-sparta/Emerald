#include "broadcastsocket.h"

BroadcastSocket::BroadcastSocket(QString datagramNeeded, QObject* parent) : QObject(parent) {
  this->datagramNeeded = datagramNeeded;
  udpSocket = new QUdpSocket(this);
  isUdpManual = true;
  connect(udpSocket, SIGNAL(readyRead()),
          this, SLOT(processPendingDatagrams()));
}

void BroadcastSocket::manualStart() {
  isUdpManual = false;
  startUDP();
}

void BroadcastSocket::startUDP() {
  if(!isUdpManual && udpSocket->state() != QUdpSocket::BoundState) {
    gotFirstMessage = false;
    bool connState = false;
    do {
      connState = udpSocket->bind(45454, QUdpSocket::ShareAddress);
    } while(!connState);
    emit write("\n   UDP Socket is listening.");
  }
}

void BroadcastSocket::processPendingDatagrams() {
  if(udpSocket->state() == QUdpSocket::BoundState) {
    while (udpSocket->hasPendingDatagrams()) {
      QByteArray datagram;
      datagram.resize(udpSocket->pendingDatagramSize());
      udpSocket->readDatagram(datagram.data(), datagram.size());
      if (!gotFirstMessage) {
        parseDatagram(QString::fromUtf8(datagram));
      }
    }
  }
}

void BroadcastSocket::parseDatagram(QString datagram) {
  if(datagramNeeded == datagram) {
    emit newDatagram(datagram);
    gotFirstMessage = true;
  }
}

void BroadcastSocket::manualClose() {
  isUdpManual = true;
  close();
}

void BroadcastSocket::close() {
  if (udpSocket->state() != QUdpSocket::UnconnectedState) {
    udpSocket->close();
    emit write("   UDP Socket is closed.\n");
  }
}
