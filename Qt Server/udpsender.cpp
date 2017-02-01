#include <QtWidgets>
#include <QtNetwork>
#include <QApplication>

#include "udpsender.h"

UdpSender::UdpSender(QObject* parent) : QObject(parent) {
  timer = new QTimer(this);
  udpSocket = new QUdpSocket(this);
  messageNo = 1;
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
    addr, 45454);
  ++messageNo;
}
