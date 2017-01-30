#include "server.h"

Server::Server(QObject* parent) : QTcpServer(parent) {
  ID = 1;
  mylogfile = new Logfile;
}

Server::~Server() {
  delete mylogfile;
}

void Server::StartServer() {
  if (!this->listen(QHostAddress::AnyIPv4, 1234)) {
    qDebug() << "Could not start server.";
  }
  else {
    qDebug() << "Server started. Listening...";
  }
}

void Server::incomingConnection(qintptr SocketDescriptor) {
  QTcpSocket* client = new QTcpSocket(this);
  client->setSocketDescriptor(SocketDescriptor);
  clients.insert(client);

  connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));
  connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));

  QString user = QString::number(ID);
  ID++;
  users[client] = user;
  foreach(QTcpSocket* client, clients) {
    client->write(QString("Server: Client " + user + " has joined.\n").toUtf8());
  }

  QString ConnectMsg = "Client " + user + " from: " + client->peerAddress().toString() + " has joined.";
  qDebug() << ConnectMsg;
  mylogfile->log_buffer(LocalTimer->GetTimeFileFormat() + " " + ConnectMsg.toStdString());
  PrintUserList();
}

void Server::readyRead() {
  QTcpSocket* client = (QTcpSocket*)sender();
  if (client->canReadLine()) {
    QString line = (client->readAll()).trimmed();

    if (users.contains(client)) {
      QString user = users[client];
      QString message = "Client " + user + ": " + line;
      qDebug() << message;
      mylogfile->log_buffer(LocalTimer->GetTimeFileFormat() + " " + message.toStdString());

      foreach(QTcpSocket* otherClient, clients) {
        if (otherClient != client) {
          otherClient->write((message + '\n').toUtf8());
        }
      }
    }
  }
}

void Server::disconnected() {
  QTcpSocket* client = (QTcpSocket*)sender();
  QString user = users[client];
  QString DisconnectMsg = "Client " + user + " disconnected. ";
  qDebug() << DisconnectMsg;
  mylogfile->log_buffer(LocalTimer->GetTimeFileFormat() + " " + DisconnectMsg.toStdString());

  clients.remove(client);
  users.remove(client);

  foreach(QTcpSocket* client, clients) {
    client->write((DisconnectMsg + '\n').toUtf8());
  }
  PrintUserList();
}

void Server::PrintUserList() {
  QStringList userList;
  foreach(QString user, users.values()) {
    userList << "Client " + user;
  }
  qDebug() << "Users online: " + userList.join(", ");
  //client->write(QString("/users:" + userList.join(",") + "\n").toUtf8());
}