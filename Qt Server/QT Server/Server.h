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
    std::unique_ptr<MessageHandler> msgHandler;
    std::unique_ptr<UdpSender> udpSender;
    std::unique_ptr<MessageConverter> msgConv;
    std::shared_ptr<MessageLogfile> log;
    std::unique_ptr<DeviceLogfile> myDeviceLogfile;
    QHostAddress uiAddress;
    std::string logBuffer;
    std::map<QTcpSocket*, std::shared_ptr<Device>> onlineDevices;
    std::vector<std::shared_ptr<Device>> addedDevices;
    void deleteAllObjects();
};

#endif
