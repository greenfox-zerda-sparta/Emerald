#ifndef SERVER_H
#define SERVER_H

#include <QStringList>
#include <QTcpServer>
#include <QTcpSocket>

#include <map>
#include <set>
#include "Logfile.h"
#include "MessageHandler.h"
#include "udpsender.h"
#include "MessageConverter.h"
#include <vector>
#include <string>
#include <memory>

class Server : public QTcpServer {
  Q_OBJECT

signals:
  void startBroadcast();
  void stopBroadcast();

public:
  Server(QObject* parent = Q_NULLPTR);
  ~Server();
  void StartServer();
  bool Server::isAdmin(QTcpSocket* socket, std::vector<unsigned char> msg);

  private slots:
  void readyRead();
  void disconnected();

protected:
  void incomingConnection(qintptr SocketDescriptor);

private:
  std::set<QTcpSocket*> socketset;
  std::map<QTcpSocket*, int> devices;
  int ID;
  int adminID;
  std::vector<unsigned char> AdminMsg;
  Logfile* mylogfile;
  std::string logbuffer;
  MyTime* LocalTimer;
  MessageHandler* msgHandler;
  UdpSender* udpsender;
  MessageConverter* msgConv;
  std::shared_ptr<std::vector<QHostAddress>> HostAddresses;
};

#endif