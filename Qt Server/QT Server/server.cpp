#include "server.h"

Server::Server(QObject* parent) : QTcpServer(parent) {
  mymessagelogfile = new MessageLogfile;
  mydevicelogfile = new DeviceLogfile;
  msgHandler = new MessageHandler;
  msgConv = new MessageConverter;
  addedDevices = mydevicelogfile->get_devices_vector();
}

Server::~Server() {
  delete mymessagelogfile;
  delete mydevicelogfile;
  delete msgHandler;
  delete msgConv;
  delete udpsender;
  for (Device* device : addedDevices) {
    if (device) { delete device; }
  }
  for (auto iter : onlineDevices) {
    if (iter.first) { delete iter.first; }
    if (iter.second) { delete iter.second; }
  }
}

void Server::InitServer() {
  if (addedDevices.size() == 0) {
    std::cout << "Please enter IP address of UI:" << std::endl;
    std::string input;
    std::getline(std::cin, input);
    uiAddress = msgConv->stringToQString(input);
    HostAddresses.push_back(uiAddress);
    addedDevices.push_back(new UI(IDs{ 255, 253, 254, 255, 255, 255 }, input, 1));
  } else {
    QHostAddress tempAddr;
    for (auto i : addedDevices) {
      tempAddr = QString::fromStdString(i->get_IP());
      HostAddresses.push_back(tempAddr);
    }
  }
  udpsender = new UdpSender(HostAddresses);
  connect(this, SIGNAL(StopUdp()), udpsender, SLOT(StopUdp()));
  connect(this, SIGNAL(StartUdp()), udpsender, SLOT(StartUdp()));
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
  connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));
  connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
  Device* newDevice;
  int index = 0;
  for (auto i : addedDevices) {
    if (i->get_IP() == msgConv->qstringToString((client->peerAddress()).toString())) {
      newDevice = i;
      HostAddresses.erase(HostAddresses.begin() + index);
      break;
    }
    ++index;
  }
  (onlineDevices)[client] = newDevice;
  std::string ConnectMsg = ((int)onlineDevices[client]->get_groupID() == 254 ? "UI" : "Device " + toString((int)onlineDevices[client]->get_groupID()));
  ConnectMsg += " from: " + msgConv->qstringToString(client->peerAddress().toString()) + " has joined.";
  std::cout << ConnectMsg << std::endl;
  mymessagelogfile->MessageLogging(LogLevel::DeviceLog, LocalTimer->GetTimeFileFormat() + " " + ConnectMsg);
}

void Server::readyRead() {
  QTcpSocket* client = (QTcpSocket*)sender();
  if (client->canReadLine()) {
    QByteArray QmsgBytes = (client->readAll());
    std::vector<unsigned char> msgBytes = msgConv->qbytearrayToCharArray(QmsgBytes);
    msgHandler->MakeCommand(addedDevices, msgBytes, onlineDevices);
  }
}

void Server::disconnected() {
  QTcpSocket* client = (QTcpSocket*)sender();
  std::string DisconnectMsg;
  if ((int)onlineDevices[client]->get_groupID() != 254) {
    DisconnectMsg = "Device " + toString((int)onlineDevices[client]->get_groupID()) + " disconnected. ";
    mymessagelogfile->MessageLogging(LogLevel::DeviceLog, LocalTimer->GetTimeFileFormat() + " " + DisconnectMsg);
  } else {
    DisconnectMsg = "UI disconnected. ";
    mymessagelogfile->MessageLogging(LogLevel::UILog, LocalTimer->GetTimeFileFormat() + " " + DisconnectMsg);
    emit StartUdp();
  }
  std::cout << DisconnectMsg << std::endl;
  HostAddresses.push_back(client->peerAddress());
  onlineDevices.erase(client);
}
