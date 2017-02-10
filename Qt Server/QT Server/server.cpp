#include "server.h"
#include "udpsender.h"
#include <iostream>
#include <vector>

Server::Server(QObject* parent) : QTcpServer(parent) {
  ID = 1;
  adminID = 0;
  AdminMsg = { 48, 48, 48, 48, 48, 48, 48, 48, 48, 10 };
  mylogfile = new Logfile;
  msgHandler = new MessageHandler;
  msgConv = new MessageConverter;
}

Server::~Server() {
  delete mylogfile;
  delete msgHandler;
  delete msgConv;
  delete udpsender;
}

void Server::AddUI() {
  std::cout << "Please enter UI IPaddress (format: xxx.x.x.x):" << std::endl;  // uncomment the lines below for manually adding UI IP
  //std::string input;
  //std::getline(std::cin, input);
  //uiAddress = msgConv->stringToQString(input);
  uiAddress = "10.27.6.158";                                         // comment this when manually adding UI IP
  HostAddresses = std::make_shared<std::vector<QHostAddress>>();
  HostAddresses->push_back(uiAddress);                                        // rethink> how to handle this HostAddress vector
}

void Server::StartServer() {
  AddUI();
  udpsender = new UdpSender(HostAddresses);
  connect(this, SIGNAL(stopBroadcast()), udpsender, SLOT(stopBroadcasting()));
  connect(this, SIGNAL(startBroadcast()), udpsender, SLOT(startBroadcasting()));

  if (!this->listen(QHostAddress::AnyIPv4, 1234)) {
    std::cerr << "Could not start server." << std::endl;
  }
  else {
    std::cout << "Server started. Listening..." << std::endl;
    if (mylogfile->get_logging_status()) {
      std::cout << "File logging is on." << std::endl;
    } else {
      std::cout << "File logging is off." << std::endl;
    }
  }
}

void Server::incomingConnection(qintptr SocketDescriptor) {
  QTcpSocket* client = new QTcpSocket(this);
  client->setSocketDescriptor(SocketDescriptor);
  socketset.insert(client);

  connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));
  connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
 
  if (client->peerAddress() == uiAddress) {
    devices[client] = 0;
    emit stopBroadcast();
  } else {
    devices[client] = ID++;
  }

  std::string ConnectMsg = "Device " + toString(devices[client]) + " from: " + msgConv->qstringToString(client->peerAddress().toString()) + " has joined.";
  std::cout << ConnectMsg << std::endl;
  mylogfile->log_buffer("Connect message " + LocalTimer->GetTimeFileFormat() + " " + ConnectMsg);
}

// Dealing with incoming messages QBYTEARRAY VERSION
void Server::readyRead() {
  QTcpSocket* client = (QTcpSocket*)sender();
  if (client->canReadLine()) {                              // if we can read from the socket
    QByteArray QmsgBytes = (client->readAll());             // read to QByteArray, remove \n
    std::vector<unsigned char> msgBytes = msgConv->qbytearrayToCharArray(QmsgBytes);
    msgHandler->splitMessage(msgBytes);                    // splitting message by byte (char)
   
    if (devices[client] == adminID) {  // if the message is from admin, send it to all other connections
      for (QTcpSocket* otherClient : socketset) {            // for now the original command from the UI is sent to the devices
        if (otherClient != client) {
          otherClient->write(QmsgBytes);
        }
      }
      std::string message = "Admin: ";               // for logging and print it to console
      for (auto iter : msgBytes) { message += toString(int(iter)) + ", "; }
      mylogfile->log_buffer("Admin message " + LocalTimer->GetTimeFileFormat() + " " + message);
      std::cout << message << std::endl;

      for (auto& item : msgHandler->getCommandMap()) {
        std::cout << item.first << ": " << int(item.second) << " | ";
      }
      std::cout << std::endl;
    }
    else {                                               // if message is from a device, print it to console for now
      std::string message = "Device " + toString(devices[client]) + ": ";
      for (auto iter : msgBytes) { message += toString(int(iter)) + ", "; }
      mylogfile->log_buffer("Device message " + LocalTimer->GetTimeFileFormat() + " " + message);
      std::cout << message << std::endl;
    }
    msgHandler->executeCmd(msgBytes);
  }
}

void Server::disconnected() {
  QTcpSocket* client = (QTcpSocket*)sender();
  std::string DisconnectMsg;
  if (devices[client] != 0) {
    DisconnectMsg = "Device " + toString(devices[client]) + " disconnected. ";
  }
  else {
    DisconnectMsg = "Admin disconnected. ";
	  emit startBroadcast();
  }
  std::cout << DisconnectMsg << std::endl;
  mylogfile->log_buffer("Disconnect message " + LocalTimer->GetTimeFileFormat() + " " + DisconnectMsg);
  socketset.erase(client);
  devices.erase(client);
}

