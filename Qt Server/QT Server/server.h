#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include "MessageConverter.h"
#include "MessageHandler.h"
#include "MessageLogfile.h"
#include "SubDevice.h"
#include "udpsender.h"
#include "UI.h"

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
    MyTime* localTimer;
    MessageHandler* msgHandler;
    UdpSender* udpSender;
    MessageConverter* msgConv;
    MessageLogfile* myMessageLogfile;
    DeviceLogfile* myDeviceLogfile;
    QHostAddress uiAddress;
    std::string messageLogBuffer;
    std::vector<QHostAddress> hostAddresses;
    std::map<QTcpSocket*, Device*> onlineDevices;
    std::vector<Device*> addedDevices;
};

#endif
