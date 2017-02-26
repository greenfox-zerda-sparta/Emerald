#ifndef SERVER_H
#define SERVER_H

#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include "Messages.h"
#include "MessageConverter.h"
#include "MessageLogfile.h"
#include "SubDevice.h"
#include "UDPSender.h"
#include "UI.h"

class MessageHandler;
class Server : public QTcpServer {
    Q_OBJECT

  signals:
    void StartUdp();
    void StopUdp();
    void Quit();

  public:
    Server(QObject* parent = Q_NULLPTR);
    ~Server();
    void RunServer();
    void InitServer();
    void StopServer();
    void ResetServer();
    void RestartServer();

  private slots:
    void readyRead();
    void disconnected();

  private:
    void incomingConnection(qintptr SocketDescriptor);
    MyTime* localTimer;
    MessageHandler* msgHandler;
    UdpSender* udpSender;
    MessageConverter* msgConv;
    MessageLogfile* log;
    DeviceLogfile* myDeviceLogfile;
    QHostAddress uiAddress;
    std::string logBuffer;
    std::map<QTcpSocket*, Device*> onlineDevices;
    std::vector<Device*> addedDevices;
};

#endif
