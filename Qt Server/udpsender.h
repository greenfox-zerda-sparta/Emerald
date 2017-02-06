#ifndef UDPSENDER_H
#define UDPSENDER_H
#include <QObject>
#include <QtNetwork>

class QTimer;
class QUdpSocket;

class UdpSender : public QObject
{
  Q_OBJECT

public:
  UdpSender(std::vector<QHostAddress> *_HostAddresses, QObject* parent = Q_NULLPTR);

private slots:
  void startBroadcasting();
  void stopBroadcasting();
  void broadcastDatagram();

private:
  QUdpSocket* udpSocket;
  QTimer* timer;
  int messageNo;
  int broadcastPort;
  std::vector<QHostAddress> *HostAddresses;
};

#endif
