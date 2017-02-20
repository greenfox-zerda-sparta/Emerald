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
  udpsender = new UdpSender(addedDevices);
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
    if (i->get_IP() == msgConv->qstringToString((client->peerAddress()).toString())) {
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
/////////////////////////////// New message to UI <- device connected
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
    QByteArray QmsgBytes = (client->readAll());
    messagelogbuffer = "Received: " + msgConv->qbytearrayToString(QmsgBytes) + " from: " + msgConv->qstringToString((client->peerAddress()).toString());
    mymessagelogfile->MessageLogging(DeviceLog, messagelogbuffer);
    if (QmsgBytes.length() < 18) {
      messagelogbuffer += "\nError: command too short.";
      mymessagelogfile->MessageLogging(Error, messagelogbuffer);
    } else {
      std::vector<unsigned char> msgBytes = msgConv->qbytearrayToCharArray(QmsgBytes);
      msgHandler->MakeCommand(addedDevices, msgBytes, onlineDevices, mymessagelogfile);
    }
  }
}

void Server::disconnected() {
  QTcpSocket* client = (QTcpSocket*)sender();
  if ((int)onlineDevices[client]->get_groupID() != 254) {
    messagelogbuffer = "Device type " + toString((int)onlineDevices[client]->get_groupID()) + " disconnected. ";
    mymessagelogfile->MessageLogging(DeviceLog, messagelogbuffer);
    /////////////////////////////// New message to UI <- device disconnected
    Messages Msg;
    std::vector<byte> msg = Msg.getMessage(249, onlineDevices[client]->get_deviceIDHigh(), onlineDevices[client]->get_deviceIDLow());
    msgHandler->MakeCommand(addedDevices, msg, onlineDevices, mymessagelogfile);
  } else {
    messagelogbuffer = "UI disconnected. ";
    mymessagelogfile->MessageLogging(DeviceLog, messagelogbuffer);
  }
  onlineDevices[client]->SetIsOnline(false);
  onlineDevices.erase(client);
}
