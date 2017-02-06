#include "udpsender.h"
#include <QDebug>
#include <QUnhandledException>

UdpSender::UdpSender(std::shared_ptr<std::vector<QHostAddress>> HostAddresses, QObject* parent) : HostAddresses(HostAddresses), QObject(parent) {
 // this->HostAddresses = HostAddresses;
  timer = new QTimer(this);
  udpSocket = new QUdpSocket(this);
  startBroadcasting();
  connect(timer, SIGNAL(timeout()), this, SLOT(broadcastDatagram()));
}

void UdpSender::startBroadcasting() {
  timer->start(1000);
}

void UdpSender::stopBroadcasting() {
	timer->stop();
}

void UdpSender::broadcastDatagram() {
  QByteArray datagram = "turquoise&emerald";
	if (!(HostAddresses)->empty()) {
      for (QHostAddress i : *HostAddresses) {
		  udpSocket->writeDatagram(datagram.data(), datagram.size(), i, 45454);
	  }
	  qDebug() << "Broadcasting message: " << QString::fromUtf8(datagram);
	}
}
