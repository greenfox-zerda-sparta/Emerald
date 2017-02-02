#include "udpsender.h"
#include <QDebug>

UdpSender::UdpSender(QObject* parent) : QObject(parent) {
  timer = new QTimer(this);
  udpSocket = new QUdpSocket(this);
  messageNo = 1;
  startBroadcasting();
  connect(timer, SIGNAL(timeout()), this, SLOT(broadcastDatagram()));
}

void UdpSender::startBroadcasting() {
  timer->start(1000);
}

void UdpSender::broadcastDatagram() {
  QByteArray datagram = "Broadcast message " + QByteArray::number(messageNo);
  udpSocket->writeDatagram(datagram.data(), datagram.size(),
	  QHostAddress::Broadcast, 45454);
  qDebug() << "Broadcasting... " << messageNo;
  ++messageNo;
  if (messageNo == 100) {
    messageNo = 0;
  }
}
