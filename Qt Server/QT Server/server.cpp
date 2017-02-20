#include "Server.h"

Server::Server(QObject* parent) : QTcpServer(parent) {
  log = new MessageLogfile;
  myDeviceLogfile = new DeviceLogfile;
  msgHandler = new MessageHandler;
  msgConv = new MessageConverter;
  addedDevices = myDeviceLogfile->GetDevicesVector();
}

Server::~Server() {
  delete log;
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
    logBuffer = "UI IP address from input: " + input;
    log->MessageLogging(Log, logBuffer);
    uiAddress = msgConv->StringToQString(input);
    addedDevices.push_back(new UI(IDs{ 255, 253, 254, 255, 255, 255 }, input, 1));
  }
  udpSender = new UdpSender(addedDevices);
  logBuffer = "UDP messaging available for devices added but not connected.";
  log->MessageLogging(Log, logBuffer);
}

void Server::RunServer() {
  InitServer();
  if (!this->listen(QHostAddress::AnyIPv4, 1234)) {
    logBuffer = "Could not start server.";
    log->MessageLogging(Error, logBuffer);
  } else {
    logBuffer = "Server started. Listening...";
    log->MessageLogging(Log, logBuffer);
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
    logBuffer = (int(onlineDevices[client]->GetGroupID()) == 254 ? "UI" : "Device " + ToString(int(onlineDevices[client]->GetGroupID())));
    logBuffer += " from: " + msgConv->QStringToString(client->peerAddress().toString()) + " has joined.";
    log->MessageLogging(DeviceLog, logBuffer);
    Messages Msg;
    std::vector<byte> msg = Msg.GetMessage(246, onlineDevices[client]->GetDeviceIDHigh(), onlineDevices[client]->GetDeviceIDLow());
    msgHandler->MakeCommand(addedDevices, msg, onlineDevices, log);
  } else {
    logBuffer = "Unauthorized connection from ip: " + msgConv->QStringToString((client->peerAddress()).toString()) + " rejected.";
    log->MessageLogging(Warning, logBuffer);
    client->close();
    delete newDevice;
  }
}

void Server::readyRead() {
  QTcpSocket* client = (QTcpSocket*)sender();
  if (client->canReadLine()) {
    QByteArray qMsgBytes = (client->readAll());
    logBuffer = "Received: " + msgConv->QByteArrayToString(qMsgBytes) + " from: " + msgConv->QStringToString((client->peerAddress()).toString());
    log->MessageLogging(LogLevel::DeviceLog, logBuffer);
    if (qMsgBytes.length() < 18) {
      logBuffer += "\nError: command too short.";
      log->MessageLogging(Error, logBuffer);
    } else {
      std::vector<unsigned char> msgBytes = msgConv->QByteArrayToCharArray(qMsgBytes);
      msgHandler->MakeCommand(addedDevices, msgBytes, onlineDevices, log);
    }
  }
}

void Server::disconnected() {
  QTcpSocket* client = (QTcpSocket*)sender();
  if (int(onlineDevices[client]->GetGroupID()) != 254) {
    logBuffer = "Device type " + ToString(int(onlineDevices[client]->GetGroupID())) + " disconnected. ";
    log->MessageLogging(DeviceLog, logBuffer);
    Messages Msg;
    std::vector<byte> msg = Msg.GetMessage(249, onlineDevices[client]->GetDeviceIDHigh(), onlineDevices[client]->GetDeviceIDLow());
    msgHandler->MakeCommand(addedDevices, msg, onlineDevices, log);
  } else {
    logBuffer = "UI disconnected. ";
    log->MessageLogging(DeviceLog, logBuffer);
  }
  onlineDevices[client]->SetIsOnline(false);
  onlineDevices.erase(client);
}
