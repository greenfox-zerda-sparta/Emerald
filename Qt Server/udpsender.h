#ifndef UDPSENDER_H
#define UDPSENDER_H

#include <QWidget>
#include <QApplication>

class QApplication;
class QTimer;
class QUdpSocket;


class UdpSender : public QObject
{
  Q_OBJECT

public:
  UdpSender(QObject* parent = Q_NULLPTR);

  private slots:
  void startBroadcasting();
  void broadcastDatagram();

private:
  QUdpSocket* udpSocket;
  QTimer* timer;
  int messageNo;
};

#endif
