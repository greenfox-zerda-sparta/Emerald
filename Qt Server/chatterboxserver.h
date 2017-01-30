#ifndef CHATTERBOXSERVER_H
#define CHATTERBOXSERVER_H

#include <QStringList>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QSet>
#include "Logfile.h"

class ChatServer : public QTcpServer {
  Q_OBJECT

public:
  ChatServer(QObject* parent = Q_NULLPTR);
  ~ChatServer();
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

#endif // CHATTERBOXSERVER_H
