#ifndef SERVER_H
#define SERVER_H

#include <map>
#include <vector>
#include <memory>
#include <iostream>
#include "MessageLogfile.h"
#include "MessageHandler.h"
#include "udpsender.h"
#include "MessageConverter.h"
#include "UI.h"
#include "SubDevice.h"
#include "udpsender.h"

class Server : public QTcpServer {
  Q_OBJECT

signals:
  void StartUdp();
  void StopUdp();

public:
  Server(QObject* parent = Q_NULLPTR);
  ~Server();
  void RunServer();
  void InitServer();

  private slots:
  void readyRead();
  void disconnected();

private:
  void incomingConnection(qintptr SocketDescriptor);
  QHostAddress uiAddress;
  MessageLogfile* mymessagelogfile;
  DeviceLogfile* mydevicelogfile;
  std::string messagelogbuffer;
  MyTime* LocalTimer;
  MessageHandler* msgHandler;
  UdpSender* udpsender;
  MessageConverter* msgConv;
  std::shared_ptr<std::vector<QHostAddress>> HostAddresses;
  std::shared_ptr<std::map<QTcpSocket*, Device>> onlineDevices;
  std::vector<Device>* addedDevices;
};

#endif
