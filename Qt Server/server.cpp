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
  //foreach(QTcpSocket* client, clients) {
  //  client->write(QString("Server: Client " + user + " has joined.\n").toUtf8());
  //}

  QString ConnectMsg = "Device " + user + " from: " + client->peerAddress().toString() + " has joined.";
  qDebug() << ConnectMsg;
  mylogfile->log_buffer(LocalTimer->GetTimeFileFormat() + " " + ConnectMsg.toStdString());
  PrintUserList();
}

// Dealing with incoming messages
void Server::readyRead() {
  QTcpSocket* client = (QTcpSocket*)sender();
  if (client->canReadLine()) {                             // if we can read from the socket
    QString line = (client->readAll()).trimmed();          // read message to Qstring
    isAdmin(client, line);                                 // if it is the "admin" message, store adminID 
    if (users[client] == "admin") {                        // if the message is from admin, send it to all other connections
      foreach(QTcpSocket* otherClient, clients) {
        if (otherClient != client) {
          otherClient->write((line + '\n').toUtf8());
        }
      }
      QString message = "Admin: " + line;                  // for logging
      mylogfile->log_buffer(LocalTimer->GetTimeFileFormat() + " " + message.toStdString());
      qDebug() << message;
    } else {                                               // if message is from a device, print it to console
      QString user = users[client];
      QString message = "Device " + user + ": " + line;
      mylogfile->log_buffer(LocalTimer->GetTimeFileFormat() + " " + message.toStdString());
      qDebug() << message;
    }
  }
}

void Server::disconnected() {
  QTcpSocket* client = (QTcpSocket*)sender();
  QString user = users[client];
  QString DisconnectMsg;
  if (user != "admin") {
    DisconnectMsg = "Device " + user + " disconnected. ";
  } else {
    DisconnectMsg = "Admin disconnected. ";
  }
  qDebug() << DisconnectMsg;
  mylogfile->log_buffer(LocalTimer->GetTimeFileFormat() + " " + DisconnectMsg.toStdString());
  clients.remove(client);
  users.remove(client);

  //foreach(QTcpSocket* client, clients) {
  //  client->write((DisconnectMsg + '\n').toUtf8());
  //}
  PrintUserList();
}

void Server::PrintUserList() {
  QStringList userList;
  foreach(QString user, users.values()) {
    if (user.toInt() != adminID) {
      userList << "Device " + user;
    }
  }
  qDebug() << "Devices online: " + userList.join(", ");
  //client->write(QString("/users:" + userList.join(",") + "\n").toUtf8());
}

// check if message sender is admin  
bool Server::isAdmin(QTcpSocket* socket, QString line) {    
  std::string linestr = line.toStdString();                // changing to std::string
  std::transform(linestr.begin(), linestr.end(), linestr.begin(), ::tolower);
  if (linestr == "admin") {                                // if first messaage is admin, store ID as adminID
    users[socket] = "admin";
    return true;
  }
  return false;
}