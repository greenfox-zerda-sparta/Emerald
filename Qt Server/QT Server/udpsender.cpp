#include "udpsender.h"
#include <QDebug>

UdpSender::UdpSender(std::vector<QHostAddress>& _HostAddresses, QObject* parent) : HostAddresses(_HostAddresses), QObject(parent) {
  datagram = "turquoise&emerald";
  timer = new QTimer(this);
  udpSocket = new QUdpSocket(this);
  udpPort = 45454;
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
	if (!HostAddresses.empty()) {
      for (QHostAddress ip : HostAddresses) {
      udpSocket->writeDatagram(datagram.data(), datagram.size(), ip, udpPort);
    //  qDebug() << "UDP to: " << ip << " Message: " << QString::fromUtf8(datagram);
    }
	}
}
