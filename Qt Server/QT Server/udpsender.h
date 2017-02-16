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
  UdpSender(std::weak_ptr<std::vector<QHostAddress>> HostAddresses, QObject* parent = Q_NULLPTR);

private slots:
  void StartUdp();
  void StopUdp();
  void SendDatagram();

private:
  QByteArray datagram;
  QUdpSocket* udpSocket;
  QTimer* timer;
  int messageNo;
  int udpPort;
  std::shared_ptr<std::vector<QHostAddress>> HostAddresses;
};

#endif
