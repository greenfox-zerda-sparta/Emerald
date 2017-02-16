#include "server.h"
#include "udpsender.h"
#include <iostream>
#include <vector>
#include <thread>

Server::Server(QObject* parent) : QTcpServer(parent) {
  ID = 1;
  adminID = 0;
  mymessagelogfile = new MessageLogfile;
  mydevicelogfile = new DeviceLogfile;
  msgHandler = new MessageHandler;
  msgConv = new MessageConverter;
  addedDevices = new std::vector<Device>;
  HostAddresses = std::make_shared<std::vector<QHostAddress>>();
  deviceMap = std::make_shared<std::map<QTcpSocket*, Device>>();
  try {
      *addedDevices = mydevicelogfile->get_devices_vector();
  } catch (...){
  }
}

Server::~Server() {
  delete mymessagelogfile;
  delete mydevicelogfile;
  delete msgHandler;
  delete msgConv;
  delete udpsender;
  delete[] addedDevices;
}

void Server::AddUI() {
  if(addedDevices->size() == 0) {
    std::cout << "Please enter IP address of UI:" << std::endl;              // uncomment the lines below for manually adding UI IP
    std::string input;
    std::getline(std::cin, input);
    uiAddress = msgConv->stringToQString(input);

//    uiAddress = "10.27.6.158";                                            // comment this when manually adding UI IP
//     uiAddress = "127.0.0.1";                                         // comment this when manually adding UI IP
    HostAddresses->push_back(uiAddress);                                        // rethink> how to handle this HostAddress vector
    addedDevices->push_back(UI(IDs{255, 253, 254, 255, 255, 255}, input));
  } else {
    QHostAddress tempAddr;
    for(auto i: *addedDevices) {
      tempAddr = QString::fromStdString(i.get_IP());
      HostAddresses->push_back(tempAddr);                                        // rethink> how to handle this HostAddress vector
    }
  }
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
  }
}

void Server::incomingConnection(qintptr SocketDescriptor) {
  QTcpSocket* client = new QTcpSocket(this);
  client->setSocketDescriptor(SocketDescriptor);
  socketset.insert(client);

  connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));
  connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
  Device newDevice;
  int index = -1;
  for(auto i: *addedDevices) {
    if(i.get_IP() == msgConv->qstringToString((client->peerAddress()).toString())){
       newDevice = i;
       ++index;
       HostAddresses->erase(HostAddresses->begin() + index);
       break;
    }
  }
  (*deviceMap)[client] = newDevice;

  std::string ConnectMsg = ((int)(*deviceMap)[client].get_groupID()==254?"UI":"Device " + toString((int)(*deviceMap)[client].get_groupID()));
  ConnectMsg += " from: " + msgConv->qstringToString(client->peerAddress().toString()) + " has joined.";
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

     msgHandler->executeCmd(addedDevices, client, msgBytes, deviceMap, msgConv);
  }
}

void Server::disconnected() {
  QTcpSocket* client = (QTcpSocket*)sender();
  std::string DisconnectMsg;
  if ((int)(*deviceMap)[client].get_groupID() != 254) {
    DisconnectMsg = "Device " + toString((int)(*deviceMap)[client].get_groupID()) + " disconnected. ";
    mymessagelogfile->message_log_buffer(DeviceLog, LocalTimer->GetTimeFileFormat() + " " + DisconnectMsg);
  }
  else {
    DisconnectMsg = "UI disconnected. ";
    mymessagelogfile->message_log_buffer(UILog, LocalTimer->GetTimeFileFormat() + " " + DisconnectMsg);
    emit startBroadcast();
  }
  std::cout << DisconnectMsg << std::endl;
  HostAddresses->push_back(client->peerAddress());
  socketset.erase(client);
  (*deviceMap).erase(client);
}

