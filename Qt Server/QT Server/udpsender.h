#ifndef UDPSENDER_H
#define UDPSENDER_H
#include <QObject>
#include <QtNetwork>
#include <memory>

class QTimer;
class QUdpSocket;

class UdpSender : public QObject
{
  Q_OBJECT

public:
  UdpSender(std::shared_ptr<std::vector<QHostAddress>> HostAddresses, QObject* parent = Q_NULLPTR);

private slots:
  void startBroadcasting();
  void stopBroadcasting();
  void broadcastDatagram();

private:
  QUdpSocket* udpSocket;
  QTimer* timer;
  int messageNo;
  int broadcastPort;
  std::shared_ptr<std::vector<QHostAddress>> HostAddresses;
};

#endif
