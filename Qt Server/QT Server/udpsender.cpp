#include "udpsender.h"
#include <QDebug>

UdpSender::UdpSender(std::vector<Device*>& addedDevices, QObject* parent) : addedDevices(addedDevices), QObject(parent) {
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

void UdpSender::SendDatagram() {
  for (Device* dev : addedDevices) {
    if(!dev->GetIsOnline() && dev->isworking()) {
      udpSocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress(QString::fromStdString(dev->get_IP())), udpPort);
      qDebug() << "UDP to: " << QString::fromStdString(dev->get_IP()) << " Message: " << QString::fromUtf8(datagram);
    }
  }
}
