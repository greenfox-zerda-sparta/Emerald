#include "server.h"

Server::Server(QObject* parent) : QTcpServer(parent) {
  mymessageLogfile = new messageLogfile;
  myDeviceLogfile = new DeviceLogfile;
  msgHandler = new MessageHandler;
  msgConv = new MessageConverter;
  addedDevices = myDeviceLogfile->GetDevicesVector();
}

Server::~Server() {
  delete mymessageLogfile;
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
    messageLogBuffer = "UI IP address from input: " + input;
    mymessageLogfile->MessageLogging(Log, messageLogBuffer);
    uiAddress = msgConv->StringToQString(input);
    addedDevices.push_back(new UI(IDs{ 255, 253, 254, 255, 255, 255 }, input, 1));
  }
  udpSender = new UdpSender(addedDevices);
  messageLogBuffer = "UDP messaging available for devices added but not connected.";
  mymessageLogfile->MessageLogging(Log, messageLogBuffer);
}

void Server::RunServer() {
  InitServer();
  if (!this->listen(QHostAddress::AnyIPv4, 1234)) {
    messageLogBuffer = "Could not start server.";
    mymessageLogfile->MessageLogging(Error, messageLogBuffer);
  } else {
    messageLogBuffer = "Server started. Listening...";
    mymessageLogfile->MessageLogging(Log, messageLogBuffer);
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
    messageLogBuffer = ((int)onlineDevices[client]->GetGroupID() == 254 ? "UI" : "Device " + ToString((int)onlineDevices[client]->GetGroupID()));
    messageLogBuffer += " from: " + msgConv->QStringToString(client->peerAddress().toString()) + " has joined.";
    mymessageLogfile->MessageLogging(DeviceLog, messageLogBuffer);
    Messages Msg;
    std::vector<byte> msg = Msg.getMessage(246, onlineDevices[client]->GetDeviceIDHigh(), onlineDevices[client]->GetDeviceIDLow());
    msgHandler->MakeCommand(addedDevices, msg, onlineDevices, mymessageLogfile);
  } else {
    messageLogBuffer = "Unauthorized connection from ip: " + msgConv->QStringToString((client->peerAddress()).toString()) + " rejected.";
    mymessageLogfile->MessageLogging(Warning, messageLogBuffer);
    client->close();
    delete newDevice;
  }
}

void Server::readyRead() {
  QTcpSocket* client = (QTcpSocket*)sender();
  if (client->canReadLine()) {
    QByteArray qMsgBytes = (client->readAll());
    messageLogBuffer = "Received: " + msgConv->QByteArrayToString(qMsgBytes) + " from: " + msgConv->QStringToString((client->peerAddress()).toString());
    mymessageLogfile->MessageLogging(LogLevel::DeviceLog, messageLogBuffer);
    if (qMsgBytes.length() < 18) {
      messageLogBuffer += "\nError: command too short.";
      mymessageLogfile->MessageLogging(Error, messageLogBuffer);
    } else {
      std::vector<unsigned char> msgBytes = msgConv->QByteArrayToCharArray(qMsgBytes);
      msgHandler->MakeCommand(addedDevices, msgBytes, onlineDevices, mymessageLogfile);
    }
  }
}

void Server::disconnected() {
  QTcpSocket* client = (QTcpSocket*)sender();
  if (int(onlineDevices[client]->GetGroupID()) != 254) {
    messageLogBuffer = "Device type " + ToString((int)onlineDevices[client]->GetGroupID()) + " disconnected. ";
    mymessageLogfile->MessageLogging(DeviceLog, messageLogBuffer);
    Messages Msg;
    std::vector<byte> msg = Msg.getMessage(249, onlineDevices[client]->GetDeviceIDHigh(), onlineDevices[client]->GetDeviceIDLow());
    msgHandler->MakeCommand(addedDevices, msg, onlineDevices, mymessageLogfile);
  } else {
    messageLogBuffer = "UI disconnected. ";
    mymessageLogfile->MessageLogging(DeviceLog, messageLogBuffer);
  }
  onlineDevices[client]->SetIsOnline(false);
  onlineDevices.erase(client);
}
