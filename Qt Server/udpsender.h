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
  UdpSender(QObject* parent = Q_NULLPTR);
  void stopBroadcasting();

private slots:
  void startBroadcasting();
  void broadcastDatagram();

private:
  QUdpSocket* udpSocket;
  QTimer* timer;
  int messageNo;
  int broadcastPort;
};

#endif
