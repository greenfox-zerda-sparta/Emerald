#include "server.h"

Server::Server(QObject* parent) : QTcpServer(parent) {
  mymessagelogfile = new MessageLogfile;
  mydevicelogfile = new DeviceLogfile;
  msgHandler = new MessageHandler;
  msgConv = new MessageConverter;
  addedDevices = new std::vector<Device>;
  HostAddresses = std::make_shared<std::vector<QHostAddress>>();
  onlineDevices = std::make_shared<std::map<QTcpSocket*, Device>>();
  *addedDevices = mydevicelogfile->get_devices_vector();
}

Server::~Server() {
  delete mymessagelogfile;
  delete mydevicelogfile;
  delete msgHandler;
  delete msgConv;
  delete udpsender;
  delete addedDevices;
}

void Server::InitServer() {
  if(addedDevices->size() == 0) {
    std::cout << "Please enter IP address of UI:" << std::endl;              // uncomment the lines below for manually adding UI IP
    std::string input;
    std::getline(std::cin, input);
    uiAddress = msgConv->stringToQString(input);
    HostAddresses->push_back(uiAddress);                                        // rethink> how to handle this HostAddress vector
    addedDevices->push_back(UI(IDs{255, 253, 254, 255, 255, 255}, input, 1));
  } else {
    QHostAddress tempAddr;
    for(auto i: *addedDevices) {
      tempAddr = QString::fromStdString(i.get_IP());
      HostAddresses->push_back(tempAddr);                                        // rethink> how to handle this HostAddress vector
    }
  }
  udpsender = new UdpSender(HostAddresses);
  connect(this, SIGNAL(StopUdp()), udpsender, SLOT(StopUdp())); //átnevezni
  connect(this, SIGNAL(StartUdp()), udpsender, SLOT(StartUdp()));
}

void Server::RunServer() {
  InitServer();
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
  connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));
  connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
  Device newDevice;
  int index = -1; // new method
  for(auto i: *addedDevices) {
    if(i.get_IP() == msgConv->qstringToString((client->peerAddress()).toString())){
       newDevice = i;
       ++index;
       HostAddresses->erase(HostAddresses->begin() + index);
       break;
    }
  }
  (*onlineDevices)[client] = newDevice;
  std::string ConnectMsg = ((int)(*onlineDevices)[client].get_groupID()==254?"UI":"Device " + toString((int)(*onlineDevices)[client].get_groupID()));
  ConnectMsg += " from: " + msgConv->qstringToString(client->peerAddress().toString()) + " has joined.";
  std::cout << ConnectMsg << std::endl;
  mymessagelogfile->MessageLogging(DeviceLog, LocalTimer->GetTimeFileFormat() + " " + ConnectMsg);
}

void Server::readyRead() {
  QTcpSocket* client = (QTcpSocket*)sender();
  if (client->canReadLine()) {
    QByteArray QmsgBytes = (client->readAll());
    std::vector<unsigned char> msgBytes = msgConv->qbytearrayToCharArray(QmsgBytes);
    msgHandler->splitMessage(msgBytes);
    msgHandler->executeCmd(addedDevices, client, msgBytes, onlineDevices, msgConv);
  }
}

void Server::disconnected() {
  QTcpSocket* client = (QTcpSocket*)sender();
  std::string DisconnectMsg;
  if ((int)(*onlineDevices)[client].get_groupID() != 254) { //disconnectlog
    DisconnectMsg = "Device " + toString((int)(*onlineDevices)[client].get_groupID()) + " disconnected. ";
    mymessagelogfile->MessageLogging(DeviceLog, LocalTimer->GetTimeFileFormat() + " " + DisconnectMsg);
  }
  else {
    DisconnectMsg = "UI disconnected. ";
    mymessagelogfile->MessageLogging(UILog, LocalTimer->GetTimeFileFormat() + " " + DisconnectMsg);
    emit StartUdp();
  }
  std::cout << DisconnectMsg << std::endl;
  HostAddresses->push_back(client->peerAddress());
  (*onlineDevices).erase(client);
}

