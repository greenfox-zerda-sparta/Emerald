#ifndef SERVER_H
#define SERVER_H

#include <QStringList>
#include <QTcpServer>
#include <QTcpSocket>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <memory>
#include "Logfile.h"
#include "MessageHandler.h"
#include "udpsender.h"
#include "MessageConverter.h"

class Server : public QTcpServer {
  Q_OBJECT

signals:
  void startBroadcast();
  void stopBroadcast();

public:
  Server(QObject* parent = Q_NULLPTR);
  ~Server();
  void StartServer();
  void Server::AddUI();
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
  QHostAddress uiAddress;
  Logfile* mylogfile;
  std::string logbuffer;
  MyTime* LocalTimer;
  MessageHandler* msgHandler;
  UdpSender* udpsender;
  MessageConverter* msgConv;
  std::shared_ptr<std::vector<QHostAddress>> HostAddresses;
};

#endif