#include "server.h"
#include "udpsender.h"

Server::Server(QObject* parent) : QTcpServer(parent) {
  ID = 1;
  mylogfile = new Logfile;
  msgHandler = new MessageHandler;
  udpsender = new UdpSender;
}

Server::~Server() {
  delete mylogfile;
  delete msgHandler;
  delete udpsender;
}

void Server::StartServer() {
  if (!this->listen(QHostAddress::AnyIPv4, 1234)) {
    qDebug() << "Could not start server.";
  }
  else {
    qDebug() << "Server started. Listening...";
    if (mylogfile->get_logging_status()) {
      qDebug() << "File logging is on.";
    } else {
      qDebug() << "File logging is off.";
    }
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

  QString ConnectMsg = "Device " + user + " from: " + client->peerAddress().toString() + " has joined.";
  qDebug() << ConnectMsg;
  mylogfile->log_buffer("Connect message " + LocalTimer->GetTimeFileFormat() + " " + ConnectMsg.toStdString());
  PrintUserList();
}

// Dealing with incoming messages QBYTEARRAY VERSION
void Server::readyRead() {
  QTcpSocket* client = (QTcpSocket*)sender();
  if (client->canReadLine()) {                             // if we can read from the socket
    QByteArray msgBytes = (client->readAll()).trimmed();   // read to QByteArray, remove \n
    msgHandler->toFullCommand(msgBytes);                   // splitting message by byte (char)
    isAdmin(client, msgBytes);                             // checking for admin

    if (users[client] == "admin" && msgBytes != "00000000") {  // if the message is from admin, send it to all other connections
      foreach(QTcpSocket* otherClient, clients) {
        if (otherClient != client) {
          otherClient->write(msgBytes);
        }
      }
      QString message = "Admin: " + msgBytes;               // for logging
      mylogfile->log_buffer("Admin message " + LocalTimer->GetTimeFileFormat() + " " + message.toStdString());
      qDebug() << message;

      for (auto item : msgHandler->getFullCommand()) {
        qDebug() << item;
      }

    }
    else {                                               // if message is from a device, print it to console
      QString user = users[client];
      QString message = "Device " + user + ": " + msgBytes;
      // foreach(QTcpSocket* otherClient, clients) {   // uncomment this only if clients do not echo back the received message!
      //   if (otherClient != client) {
      //     otherClient->write(message.toUtf8());
      //    }
      //  }
      mylogfile->log_buffer("Device message " + LocalTimer->GetTimeFileFormat() + " " + message.toStdString());
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
  }
  else {
    DisconnectMsg = "Admin disconnected. ";
  }
  qDebug() << DisconnectMsg;
  mylogfile->log_buffer("Disconnect message " + LocalTimer->GetTimeFileFormat() + " " + DisconnectMsg.toStdString());
  clients.remove(client);
  users.remove(client);
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
}

// check if message sender is admin QBYTEARRAY VERSION
bool Server::isAdmin(QTcpSocket* socket, QByteArray bytes) {
  QString adminID = "00000000";
  if (bytes.trimmed() == adminID) {
    users[socket] = "admin";
    return true;
  }
  return false;
}
