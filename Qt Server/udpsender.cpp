#include "udpsender.h"
#include <QDebug>
#include <QUnhandledException>

UdpSender::UdpSender(std::vector<QHostAddress> *HostAddresses, QObject* parent) : QObject(parent) {
  this->HostAddresses = &HostAddresses;
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
  try
  {
	  if (!(*HostAddresses)->empty()) {
		  for (int i = 0; i < (*(*HostAddresses)).size(); ++i) {
			  qDebug() << (*(*HostAddresses)).size();
			  udpSocket->writeDatagram(datagram.data(), datagram.size(),
				  (*(*HostAddresses))[i], 45454);
		  }
		  qDebug() << "Broadcasting message: " << QString::fromUtf8(datagram);
	  }
  }
  catch (QUnhandledException &e)
  {
	  qDebug() << e.what();
  }
}
