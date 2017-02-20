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
    messagelogbuffer = "UI IP address from input: " + input;
    mymessagelogfile->MessageLogging(Log, messagelogbuffer);
    uiAddress = msgConv->stringToQString(input);
    addedDevices.push_back(new UI(IDs{ 255, 253, 254, 255, 255, 255 }, input, 1));
  }
  udpSender = new UdpSender(addedDevices);
  messagelogbuffer = "UDP messaging available for devices added but not connected.";
  mymessagelogfile->MessageLogging(Log, messagelogbuffer);
}

void Server::RunServer() {
  InitServer();
  if (!this->listen(QHostAddress::AnyIPv4, 1234)) {
    messagelogbuffer = "Could not start server.";
    mymessagelogfile->MessageLogging(Error, messagelogbuffer);
  } else {
    messagelogbuffer = "Server started. Listening...";
    mymessagelogfile->MessageLogging(Log, messagelogbuffer);
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
    messagelogbuffer = ((int)onlineDevices[client]->get_groupID() == 254 ? "UI" : "Device " + toString((int)onlineDevices[client]->get_groupID()));
    messagelogbuffer += " from: " + msgConv->qstringToString(client->peerAddress().toString()) + " has joined.";
    mymessagelogfile->MessageLogging(DeviceLog, messagelogbuffer);
    Messages Msg;
    std::vector<byte> msg = Msg.getMessage(246, onlineDevices[client]->GetDeviceIDHigh(), onlineDevices[client]->GetDeviceIDLow());
    msgHandler->MakeCommand(addedDevices, msg, onlineDevices, myMessageLogfile);
  } else {
    messagelogbuffer = "Unauthorized connection from ip: " + msgConv->qstringToString((client->peerAddress()).toString()) + " rejected.";
    mymessagelogfile->MessageLogging(Warning, messagelogbuffer);
    client->close();
    delete newDevice;
  }
}

void Server::readyRead() {
  QTcpSocket* client = (QTcpSocket*)sender();
  if (client->canReadLine()) {
    QByteArray qMsgBytes = (client->readAll());
    messageLogBuffer = "Received: " + msgConv->QByteArrayToString(qMsgBytes) + " from: " + msgConv->QStringToString((client->peerAddress()).toString());
    myMessageLogfile->MessageLogging(LogLevel::DeviceLog, messageLogBuffer);
    if (qMsgBytes.length() < 18) {
      messageLogBuffer += "\nError: command too short.";
      myMessageLogfile->MessageLogging(Error, messageLogBuffer);
    } else {
      std::vector<unsigned char> msgBytes = msgConv->QByteArrayToCharArray(qMsgBytes);
      msgHandler->MakeCommand(addedDevices, msgBytes, onlineDevices, myMessageLogfile);
    }
  }
}

void Server::disconnected() {
  QTcpSocket* client = (QTcpSocket*)sender();
  if (int(onlineDevices[client]->GetGroupID()) != 254) {
    messagelogbuffer = "Device type " + toString((int)onlineDevices[client]->get_groupID()) + " disconnected. ";
    mymessagelogfile->MessageLogging(DeviceLog, messagelogbuffer);
    Messages Msg;
    std::vector<byte> msg = Msg.getMessage(249, onlineDevices[client]->GetDeviceIDHigh(), onlineDevices[client]->GetDeviceIDLow());
    msgHandler->MakeCommand(addedDevices, msg, onlineDevices, mymessagelogfile);
  } else {
    messagelogbuffer = "UI disconnected. ";
    mymessagelogfile->MessageLogging(DeviceLog, messagelogbuffer);
  }
  onlineDevices[client]->SetIsOnline(false);
  onlineDevices.erase(client);
}
