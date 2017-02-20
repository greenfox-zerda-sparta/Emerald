#include "server.h"

Server::Server(QObject* parent) : QTcpServer(parent) {
  myMessageLogfile = new MessageLogfile;
  myDeviceLogfile = new DeviceLogfile;
  msgHandler = new MessageHandler;
  msgConv = new MessageConverter;
  addedDevices = myDeviceLogfile->getDevicesVector();
}

Server::~Server() {
  delete myMessageLogfile;
  delete myDeviceLogfile;
  delete msgHandler;
  delete msgConv;
  delete udpSender;
  for (Device* device : addedDevices) {
    if (device) {
      delete device;
    }
  }
  for (auto iter : onlineDevices) {
    if (iter.first) {
      delete iter.first;
    }
    if (iter.second) {
      delete iter.second;
    }
  }
}

void Server::InitServer() {
  if (addedDevices.size() == 0) {
    std::cout << "Please enter IP address of UI:" << std::endl;
    std::string input;
    std::getline(std::cin, input);
    uiAddress = msgConv->StringToQString(input);
    ////////////////////////////////////////////////// hostAddresses helyett addedDevices-t fogunk haszna'lni
    hostAddresses.push_back(uiAddress);
    addedDevices.push_back(new UI(IDs{ 255, 253, 254, 255, 255, 255 }, input, 1));
  } else {
    QHostAddress tempAddr;
    for (auto i : addedDevices) {
      tempAddr = QString::fromStdString(i->GetIP());
      //////////////////////////////////////////////////
      hostAddresses.push_back(tempAddr);
    }
  }
  //////////////////////////////////////////////////
  udpSender = new UdpSender(hostAddresses);
  connect(this, SIGNAL(StopUdp()), udpSender, SLOT(StopUdp()));
  connect(this, SIGNAL(StartUdp()), udpSender, SLOT(StartUdp()));
}

void Server::RunServer() {
  InitServer();
  if (!this->listen(QHostAddress::AnyIPv4, 1234)) {
    std::cerr << "Could not start server." << std::endl;
  } else {
    std::cout << "Server started. Listening..." << std::endl;
  }
}

void Server::incomingConnection(qintptr SocketDescriptor) {
  QTcpSocket* client = new QTcpSocket(this);
  client->setSocketDescriptor(SocketDescriptor);
  Device* newDevice;
  bool isExistingDevice = false;
  int index = 0;
  for (auto i : addedDevices) {
    if (i->GetIP() == msgConv->QStringToString((client->peerAddress()).toString())) {
      newDevice = i;
      //////////////////////////////////////////////////
      hostAddresses.erase(hostAddresses.begin() + index);
      isExistingDevice = true;
      break;
    }
    ++index;
  }
  if (isExistingDevice) {
      onlineDevices[client] = newDevice;
      connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));
      connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
      std::string connectMsg = ((int)onlineDevices[client]->GetGroupID() == 254 ? "UI" : "Device " + ToString((int)onlineDevices[client]->GetGroupID()));
      connectMsg += " from: " + msgConv->QStringToString(client->peerAddress().toString()) + " has joined.";
      std::cout << connectMsg << std::endl;
      myMessageLogfile->MessageLogging(LogLevel::DeviceLog, localTimer->GetTimeFileFormat() + " " + connectMsg);
      } else {
      std::cout << "Unauthorized connection from ip: " << msgConv->QStringToString((client->peerAddress()).toString()) << " rejected." << std::endl;
      client->close();
  }
}

void Server::readyRead() {
  QTcpSocket* client = (QTcpSocket*)sender();
  if (client->canReadLine()) {
    QByteArray qMsgBytes = (client->readAll());
    if (qMsgBytes.length() < 18) {
      //log
      std::cerr << "Error: command too short." << std::endl;
    } else {
      std::vector<unsigned char> msgBytes = msgConv->QByteArrayToCharArray(qMsgBytes);
      msgHandler->MakeCommand(addedDevices, msgBytes, onlineDevices);
    }
  }
}

void Server::disconnected() {
  QTcpSocket* client = (QTcpSocket*)sender();
  std::string disconnectMsg;
  if (int(onlineDevices[client]->GetGroupID()) != 254) {
    disconnectMsg = "Device " + ToString(int(onlineDevices[client]->GetGroupID())) + " disconnected. ";
    myMessageLogfile->MessageLogging(LogLevel::DeviceLog, localTimer->GetTimeFileFormat() + " " + disconnectMsg);
  } else {
    disconnectMsg = "UI disconnected. ";
    myMessageLogfile->MessageLogging(LogLevel::UILog, localTimer->GetTimeFileFormat() + " " + disconnectMsg);
    emit StartUdp();
  }
  std::cout << disconnectMsg << std::endl;
//////////////////////////////////////////////////
  hostAddresses.push_back(client->peerAddress());
  onlineDevices.erase(client);
}
