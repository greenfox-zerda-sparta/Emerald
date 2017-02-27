#include "Server.h"
#include "MessageHandler.h"

Server::Server(QObject* parent) : QTcpServer(parent) {
  msgHandler = std::unique_ptr<MessageHandler>(new MessageHandler(this));
}

Server::~Server() {
  deleteAllObjects();
}

void Server::deleteAllObjects() {
  for (auto iter : onlineDevices) {
    if (iter.first) {
      iter.first->disconnect();
      iter.first->abort();
      delete iter.first;
    }
  }
  onlineDevices.clear();
  this->close();
}

void Server::InitServer() {
  myDeviceLogfile = std::unique_ptr<DeviceLogfile>(new DeviceLogfile);
  log = std::shared_ptr<MessageLogfile>(new MessageLogfile);
  msgConv = std::unique_ptr<MessageConverter>(new MessageConverter);
  addedDevices = myDeviceLogfile->GetDevicesVector();
  if (addedDevices.size() == 0) {
    std::cout << "Please enter IP address of UI:" << std::endl;
    std::string input;
    std::getline(std::cin, input);
    logBuffer = "UI IP address from input: " + input;
    log->MessageLogging(Log, logBuffer);
    uiAddress = msgConv->StringToQString(input);
    addedDevices.push_back(std::shared_ptr<Device>(new UI(IDs{ 255, 253, 254, 255, 255, 255 }, input, 1)));
  }
  udpSender = std::unique_ptr<UdpSender>(new UdpSender(addedDevices));
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
  std::shared_ptr<Device> newDevice;
  bool isExistingDevice = false;
  int index = 0;
  for (auto &i : addedDevices) {
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
    logBuffer = (int(onlineDevices[client]->GetGroupID()) == 254 ? "UI" : "Device type: " + ToString(int(onlineDevices[client]->GetGroupID())));
    logBuffer += " from: " + msgConv->QStringToString(client->peerAddress().toString()) + " has joined.";
    log->MessageLogging(DeviceLog, logBuffer);
    if (int(onlineDevices[client]->GetGroupID()) != 254) {
      Messages Msg;
      std::vector<byte> msg = Msg.GetMessage(246, onlineDevices[client]->GetDeviceIDHigh(), onlineDevices[client]->GetDeviceIDLow(), onlineDevices[client]->GetFloorID(), onlineDevices[client]->GetRoomID(), onlineDevices[client]->GetGroupID());
      msgHandler->MakeCommand(addedDevices, msg, onlineDevices, log);
    }
  } else {
    logBuffer = "Unauthorized connection from ip: " + msgConv->QStringToString((client->peerAddress()).toString()) + " rejected.";
    log->MessageLogging(Warning, logBuffer);
    client->close();
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

void Server::ResetServer() {
  myDeviceLogfile->ClearLogfile();
  deleteAllObjects();
  std::cout << std::endl << "                               ..." << std::endl << std::endl;
  RunServer();
}

void Server::StopServer() {
  std::cout << std::endl << "                               ..." << std::endl << std::endl;
  emit Quit();
}

void Server::RestartServer() {
  deleteAllObjects();
  std::cout << std::endl << "                               ..." << std::endl << std::endl;
  RunServer();
}
