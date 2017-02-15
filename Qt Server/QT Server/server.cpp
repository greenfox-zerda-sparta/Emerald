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
  addedDevices = new std::vector<Device>;
  HostAddresses = std::make_shared<std::vector<QHostAddress>>();
  try {
      *addedDevices = mymessagelogfile->get_devices_vector();
  } catch (...){}
}

Server::~Server() {
  delete mymessagelogfile;
  delete msgHandler;
  delete msgConv;
  delete udpsender;
  delete[] addedDevices;
}                                                  

void Server::AddUI() {
  if(addedDevices->size() == 0) {
/*    std::cout << "Please enter UI IPaddress:" << std::endl;              // uncomment the lines below for manually adding UI IP
    std::string input;
    std::getline(std::cin, input);
    uiAddress = msgConv->stringToQString(input);
  byte deviceIDHigh;
  byte deviceIDLow;
  byte groupID;
  byte homeID;
  byte floorID;
  byte roomID;
*/
    addedDevices->push_back(Device(IDs{255, 253, 254, 255, 255, 255}, msgConv->qstringToString(uiAddress.toString())));
//    uiAddress = "10.27.6.158";                                            // comment this when manually adding UI IP
     uiAddress = "127.0.0.1";                                         // comment this when manually adding UI IP
  } else {
    for(auto i: *addedDevices) {
        if(i.get_groupID() == 254){
            uiAddress = QString::fromStdString(i.get_IP());
            break;
        }
    }
  }
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
  Device newDevice;
  for(auto i: *addedDevices) {
    if(i.get_IP() == msgConv->qstringToString((client->peerAddress()).toString())){
       newDevice = i;
       break;
    }
  }

  if (client->peerAddress() == uiAddress) {
//    devices[client] = 0;
    if(newDevice.get_IP().size() > 0) {
      (*deviceMap)[client] = newDevice;
    }
    emit stopBroadcast();
  } else {
//    devices[client] = ID++;
  }

//  std::string ConnectMsg = "Device " + toString(devices[client]) + " from: " + msgConv->qstringToString(client->peerAddress().toString()) + " has joined.";
  std::string ConnectMsg = "Device " + toString((*deviceMap)[client].get_groupID()) + " from: " + msgConv->qstringToString(client->peerAddress().toString()) + " has joined.";
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
    
   // std::map<QTcpSocket*, int>* ptr_socketmap = &deviceMap;  // ptr for devices map needed for msg transfer, get socket by device ID
    msgHandler->executeCmd(addedDevices, client, msgBytes, deviceMap, msgConv);
//    msgHandler->executeCmd(client, msgBytes, ptr_socketmap, msgConv);
  }
}

void Server::disconnected() {
  QTcpSocket* client = (QTcpSocket*)sender();
  std::string DisconnectMsg;
  if ((*deviceMap)[client].get_groupID() != 254) {
    DisconnectMsg = "Device " + toString((*deviceMap)[client].get_groupID()) + " disconnected. ";
    mymessagelogfile->message_log_buffer(DeviceLog, LocalTimer->GetTimeFileFormat() + " " + DisconnectMsg);
  }
  else {
    DisconnectMsg = "UI disconnected. ";
    mymessagelogfile->message_log_buffer(UILog, LocalTimer->GetTimeFileFormat() + " " + DisconnectMsg);
	  emit startBroadcast();
  }
  std::cout << DisconnectMsg << std::endl;
  socketset.erase(client);
  (*deviceMap).erase(client);
}

