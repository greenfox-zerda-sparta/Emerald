#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include "MessageConverter.h"
#include "MessageLogfile.h"
#include "MessageHandler.h"
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
    MyTime* LocalTimer;
    MessageHandler* msgHandler;
    UdpSender* udpsender;
    MessageConverter* msgConv;
    MessageLogfile* mymessagelogfile;
    DeviceLogfile* mydevicelogfile;
    QHostAddress uiAddress;
    std::string messagelogbuffer;
    std::map<QTcpSocket*, Device*> onlineDevices;
    std::vector<Device*> addedDevices;
};

#endif
