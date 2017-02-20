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
    addedDevices.push_back(new UI(IDs{ 255, 253, 254, 255, 255, 255 }, input, 1));


  }
  udpSender = new UdpSender(addedDevices);
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
      i->SetIsOnline(true);
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
    connectMsg += " from: " + msgConv->QStringToString(client->peerAddress().ToString()) + " has joined.";
      myMessageLogfile->MessageLogging(LogLevel::DeviceLog, ConnectMsg);
/////////////////////////////// New message to UI <- device connected
  } else {
    std::cout << "Unauthorized connection from ip: " << msgConv->QStringToString((client->peerAddress()).ToString()) << " rejected." << std::endl;
    client->close();
    delete newDevice;
  }
}

void Server::readyRead() {
  QTcpSocket* client = (QTcpSocket*)sender();
  if (client->canReadLine()) {
    QByteArray qMsgBytes = (client->readAll());
    messageLogBuffer = "Received: " + msgConv->QByteArrayToString(qMsgBytes) + " from: " + msgConv->QStringToString((client->peerAddress()).ToString());
    myMessageLogfile->MessageLogging(LogLevel::DeviceLog, messageLogBuffer);
      messageLogBuffer += "\nError: command too short.";
      myMessageLogfile->MessageLogging(LogLevel::DeviceLog, messageLogBuffer);
    } else {
      std::vector<unsigned char> msgBytes = msgConv->QByteArrayToCharArray(qMsgBytes);
      msgHandler->MakeCommand(addedDevices, msgBytes, onlineDevices, myMessageLogfile);
    }
  }
}

void Server::disconnected() {
  QTcpSocket* client = (QTcpSocket*)sender();
  std::string disconnectMsg;
  if (int(onlineDevices[client]->GetGroupID()) != 254) {
    disconnectMsg = "Device type " + ToString((int)onlineDevices[client]->GetGroupID()) + " disconnected. ";
    myMessageLogfile->MessageLogging(LogLevel::DeviceLog, disconnectMsg);
    /////////////////////////////// New message to UI <- device disconnected
    Messages Msg;
    std::vector<byte> msg = Msg.getMessage(249, onlineDevices[client]->GetDeviceIDHigh(), onlineDevices[client]->GetDeviceIDLow());
    msgHandler->MakeCommand(addedDevices, msg, onlineDevices);
  } else {
    disconnectMsg = "UI disconnected. ";
    myMessageLogfile->MessageLogging(LogLevel::UILog, disconnectMsg);
  //  emit StartUdp();
  }
  onlineDevices[client]->SetIsOnline(false);
  onlineDevices.erase(client);
}
