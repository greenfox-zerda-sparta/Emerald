#include "server.h"
#include "udpsender.h"
#include <iostream>
#include <vector>
#include <thread>

Server::Server(QObject* parent) : QTcpServer(parent) {
  ID = 1;
  adminID = 0;
  mymessagelogfile = new Logfile;
  msgHandler = new MessageHandler;
  msgConv = new MessageConverter;
}

Server::~Server() {
  delete mymessagelogfile;
  delete msgHandler;
  delete msgConv;
  delete udpsender;
}                                                  

void Server::AddUI() {
  //std::cout << "Please enter UI IPaddress:" << std::endl;              // uncomment the lines below for manually adding UI IP
  //std::string input;
  //std::getline(std::cin, input);
  //uiAddress = msgConv->stringToQString(input);
  uiAddress = "10.27.6.158";                                            // comment this when manually adding UI IP
  uiAddress = "127.0.0.1";                                         // comment this when manually adding UI IP
  HostAddresses = std::make_shared<std::vector<QHostAddress>>();
  HostAddresses->push_back(uiAddress);                                        // rethink> how to handle this HostAddress vector
}

void Server::StartServer() {
  std::cout << std::thread::hardware_concurrency() << std::endl;
  AddUI();
  udpsender = new UdpSender(HostAddresses);
  connect(this, SIGNAL(stopBroadcast()), udpsender, SLOT(stopBroadcasting()));
  connect(this, SIGNAL(startBroadcast()), udpsender, SLOT(startBroadcasting()));
  std::cout << std::thread::hardware_concurrency() << std::endl;
  if (!this->listen(QHostAddress::AnyIPv4, 1234)) {
    std::cerr << "Could not start server." << std::endl;
  }
  else {
    std::cout << "Server started. Listening..." << std::endl;
    if (mymessagelogfile->get_logging_status()) {
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
  mymessagelogfile->message_log_buffer(DeviceLog, LocalTimer->GetTimeFileFormat() + " " + ConnectMsg);
}

// Dealing with incoming messages QBYTEARRAY VERSION
void Server::readyRead() {
  QTcpSocket* client = (QTcpSocket*)sender();
  if (client->canReadLine()) {                              // if we can read from the socket
    QByteArray QmsgBytes = (client->readAll());             // read to QByteArray, remove \n
    std::vector<unsigned char> msgBytes = msgConv->qbytearrayToCharArray(QmsgBytes);
    msgHandler->splitMessage(msgBytes);                    // splitting message by byte (char)
    
    std::map<QTcpSocket*, int>* ptr_socketmap = &devices;
    msgHandler->executeCmd(client, msgBytes, ptr_socketmap, msgConv);
    
  }
}

void Server::disconnected() {
  QTcpSocket* client = (QTcpSocket*)sender();
  std::string DisconnectMsg;
  if (devices[client] != 0) {
    DisconnectMsg = "Device " + toString(devices[client]) + " disconnected. ";
    mymessagelogfile->message_log_buffer(DeviceLog, LocalTimer->GetTimeFileFormat() + " " + DisconnectMsg);
  }
  else {
    DisconnectMsg = "Admin disconnected. ";
    mymessagelogfile->message_log_buffer(UILog, LocalTimer->GetTimeFileFormat() + " " + DisconnectMsg);
	  emit startBroadcast();
  }
  std::cout << DisconnectMsg << std::endl;
  socketset.erase(client);
  devices.erase(client);
}

