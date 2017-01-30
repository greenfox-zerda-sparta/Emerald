#ifndef SERVER_H
#define SERVER_H

#include <QStringList>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QSet>
#include "Logfile.h"

class Server : public QTcpServer {
  Q_OBJECT

public:
  Server(QObject* parent = Q_NULLPTR);
  ~Server();
  void StartServer();

  private slots:
  void readyRead();
  void disconnected();
  void PrintUserList();

protected:
  void incomingConnection(qintptr SocketDescriptor);

private:
  QSet<QTcpSocket*> clients;
  QMap<QTcpSocket*, QString> users;
  int ID;
  Logfile* mylogfile;
  std::string logbuffer;
  MyTime* LocalTimer;
};

#endif // SERVER_H
