#include "udpsender.h"
#include <QDebug>

UdpSender::UdpSender(std::weak_ptr<std::vector<QHostAddress>> HostAddresses, QObject* parent) : HostAddresses(HostAddresses), QObject(parent) {
  datagram = "turquoise&emerald";
  timer = new QTimer(this);
  udpSocket = new QUdpSocket(this);
  StartUdp();
  connect(timer, SIGNAL(timeout()), this, SLOT(SendDatagram()));
}

void UdpSender::StartUdp() {
  timer->start(1000);
}

void UdpSender::StopUdp() {
	timer->stop();
}

void UdpSender::SendDatagram() {
	if (!(HostAddresses)->empty()) {
      for (QHostAddress ip : *HostAddresses) {
      udpSocket->writeDatagram(datagram.data(), datagram.size(), ip, 45454);
      qDebug() << "UDP to: " << ip << " Message: " << QString::fromUtf8(datagram);
    }
	}
}
