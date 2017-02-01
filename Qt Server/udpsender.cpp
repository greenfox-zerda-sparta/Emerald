#include "udpsender.h"
#include <QDebug>

UdpSender::UdpSender(QObject* parent) : QObject(parent) {
  timer = new QTimer(this);
  udpSocket = new QUdpSocket(this);
  messageNo = 1;
  startBroadcasting();
  broadcastPort = 45454;
  connect(timer, SIGNAL(timeout()), this, SLOT(broadcastDatagram()));
}

void UdpSender::startBroadcasting() {
  timer->start(1000);
}

void UdpSender::broadcastDatagram() {
  QHostAddress addr;
  addr.setAddress("10.27.6.255");
  QByteArray datagram = "Broadcast message " + QByteArray::number(messageNo);
  udpSocket->writeDatagram(datagram.data(), datagram.size(),
    addr, broadcastPort);
  qDebug() << "Broadcasting... " << messageNo;
  ++messageNo;
  if (messageNo == 100) {
    messageNo = 0;
  }
}
