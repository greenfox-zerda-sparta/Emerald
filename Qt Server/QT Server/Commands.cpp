#include "Commands.h"
#include "Server.h"

Commands::Commands(std::vector<std::shared_ptr<Device>>& _addedDevices, std::shared_ptr<MessageLogfile> msgLog, Server* server)
  : addedDevs(_addedDevices), msgLogger(msgLog) {
  deviceLog = new DeviceLogfile;
  msgConvert = new MessageConverter;

  ptr_resetServer = &Commands::ResetServer;
  ptr_restartServer = &Commands::RestartServer;
  ptr_stopServer = &Commands::StopServer;
  ptr_addDevice = &Commands::AddDevice;
  ptr_removeDevice = &Commands::RemoveDevice;
  ptr_getStatusReport = &Commands::GetStatusReport;
  ptr_statusReport = &Commands::StatusReport;
  ptr_setData = &Commands::SetData;
  ptr_cmdReplyError = &Commands::CommandReplyError;
  ptr_cmdReplySuccess = &Commands::CommandReplySuccess;
  ptr_forwardMessage = &Commands::ForwardMessage;
  ptr_devforwardMessageToUi = &Commands::DevForwardMessageToUi;
  cmdMap[239] = ptr_setData;
  cmdMap[240] = ptr_cmdReplyError;
  cmdMap[241] = ptr_cmdReplySuccess;
  cmdMap[242] = ptr_statusReport;
  cmdMap[246] = ptr_getStatusReport;
  cmdMap[247] = ptr_addDevice;
  cmdMap[248] = ptr_removeDevice;
  cmdMap[253] = ptr_resetServer;
  cmdMap[254] = ptr_restartServer;
  cmdMap[255] = ptr_stopServer;
  cmdMap[249] = ptr_devforwardMessageToUi;
  cmdMap[0] = ptr_forwardMessage;
  cmdMap[1] = ptr_forwardMessage;
  cmdMap[2] = ptr_forwardMessage;
  cmdMap[3] = ptr_forwardMessage;
  cmdMap[4] = ptr_forwardMessage;
  cmdMap[5] = ptr_forwardMessage;
  this->server = server;
}

Commands::~Commands() {
  delete msgConvert;
  delete deviceLog;
}

void Commands::SetMessageMap(std::map<std::string, byte>& _messageMap) {
  this->messageMap = _messageMap;
}

void Commands::SetAddedDevices(std::vector<std::shared_ptr<Device>>& _addedDevices) {
  this->addedDevs = _addedDevices;
}

void Commands::SetDeviceMap(std::map<QTcpSocket*, std::shared_ptr<Device> >& _deviceMap) {
  this->deviceMap = _deviceMap;
}

void Commands::SetBytes(std::vector<byte>& _bytes) {
  this->bytes = _bytes;
}

void Commands::RunCommand() {
  if (cmdMap.count(messageMap["cmdID"]) < 1) {
    CommandReplyError();
  } else {
    (this->*cmdMap[messageMap["cmdID"]])();
  }
}

bool Commands::IsSenderUi() {
  return (messageMap["senderIDHigh"] == 255 && messageMap["senderIDLow"] == 253);
}

bool Commands::IsServerCommand() {
  return (messageMap["targetIDHigh"] == 255 && messageMap["targetIDLow"] == 254 && IsSenderUi());
}

void Commands::ResetServer() {
  if (IsServerCommand()) {
    msgLog = "RESETTING SERVER";
    msgLogger->MessageLogging(Log, msgLog);
    server->ResetServer();
  } else {
    CommandReplyError();
  }
}

void Commands::RestartServer() {
  if (IsServerCommand()) {
    msgLog = "RESTARTING SERVER";
    msgLogger->MessageLogging(Log, msgLog);
    server->RestartServer();
  } else {
    CommandReplyError();
  }
}

void Commands::StopServer() {
  if (IsServerCommand()) {
    msgLog = "STOPPING SERVER";
    msgLogger->MessageLogging(Log, msgLog);
    server->StopServer();
  } else {
    CommandReplyError();
  }
}

std::vector<byte> Commands::MakeUiFeedback(byte cmd, byte body1, byte body2) {
  return { 255, 253, cmd, 255, 255, 255, 254, 255, 254, body1, body2, 0, 0, 0, 0, 0, 0 };
}

void Commands::GenerateNextIDs() {
  if (addedDevs.size() <= 1) {
    IDLow = 1;
    IDHigh = 0;
  } else {
    IDHigh = int(addedDevs[addedDevs.size() - 1]->GetDeviceIDHigh());
    IDLow = int(addedDevs[addedDevs.size() - 1]->GetDeviceIDLow()) + 1;
    if (IDLow == 253) {
      IDHigh++;
      IDLow = 0;
    }
  }
}

std::string Commands::GetIPString() {
  return msgConvert->ByteToString(messageMap["body1"]) + "." +
         msgConvert->ByteToString(messageMap["body2"]) + "." +
         msgConvert->ByteToString(messageMap["body3"]) + "." +
         msgConvert->ByteToString(messageMap["body4"]);
}

std::string Commands::GetDeviceText(std::shared_ptr<Device> dev) {
  return msgConvert->ByteToString(dev->GetDeviceIDHigh()) + " " +
         msgConvert->ByteToString(dev->GetDeviceIDLow()) + " " +
         msgConvert->ByteToString(dev->GetGroupID()) + " " +
         msgConvert->ByteToString(dev->GetHomeID()) + " " +
         msgConvert->ByteToString(dev->GetFloorID()) + " " +
         msgConvert->ByteToString(dev->GetRoomID()) + " " +
         ToString(dev->GetIP()) + " " +
         msgConvert->ByteToString(dev->IsWorking());
}

bool Commands::IsRoomForDevice() {
  return (IDHigh < 252 && IDLow < 252);
}

void Commands::LogDeviceList() {
  for (std::shared_ptr<Device> device : addedDevs) {
    deviceLogBuffer += GetDeviceText(device) + "\n";
  }
  deviceLog->DeviceLogging(deviceLogBuffer);
}

void Commands::AddDevice() {
  if (IsServerCommand()) {
    if (messageMap["groupID"] != 254 && messageMap["groupID"] != 254) {
      GenerateNextIDs();
      if (IsRoomForDevice()) {
        messageMap["deviceIDHigh"] = (byte)IDHigh;
        messageMap["deviceIDLow"] = (byte)IDLow;
        std::string IP = GetIPString();
        std::shared_ptr<Device> newDevice = std::shared_ptr<Device>(new Device(messageMap, IP));
        addedDevs.push_back(newDevice);
        LogDeviceList();
        msgLog = "Command Success. Adding Device: " + GetDeviceText(newDevice);
        msgLogger->MessageLogging(LogLevel::DeviceLog, msgLog);
        bytes = MakeUiFeedback(241, (byte)IDHigh, (byte)IDLow);
        DevForwardMessageToUi();
      } else { // no more devices can be added
        CommandReplyError();
      }
    }
  } else {
    CommandReplyError();
  }
}

void Commands::RemoveDevice() {
  if (IsServerCommand()) {
    for (unsigned int i = 0; i < addedDevs.size(); i++) {
      if (addedDevs[i]->GetDeviceIDHigh() == messageMap["body1"] &&
          addedDevs[i]->GetDeviceIDLow() == messageMap["body2"]) {
        msgLog = "Removing Device: " + GetDeviceText(addedDevs[i]);
        addedDevs.erase(addedDevs.begin() + i);
        msgLogger->MessageLogging(DeviceLog, msgLog);
        CommandReplySuccess();
      }
    }
    LogDeviceList();
  } else {
    CommandReplyError();
  }
}

void Commands::GetStatusReport() {
if (IsSenderUi() || (messageMap["senderIDHigh"] == 255 && messageMap["senderIDLow"] == 254)) {
  msgLog = "Requesting status report.";
  msgLogger->MessageLogging(DeviceLog, msgLog);
  ForwardMessage();
  } else {
  CommandReplyError();
  }
}

void Commands::StatusReport() {
  msgLog = "Received status report.";
  msgLogger->MessageLogging(DeviceLog, msgLog);
  DevForwardMessageToUi();
}

void Commands::SetData() {
  if (IsSenderUi()) {
    msgLog = "Set device data.";
    msgLogger->MessageLogging(DeviceLog, msgLog);
  } else {
    CommandReplyError();
  }
}

void Commands::CommandReplyError() {
  msgLog = "Error: invalid command.";
  msgLogger->MessageLogging(DeviceLog, msgLog);
  bytes = MakeUiFeedback(240, 0, 0);
  DevForwardMessageToUi();
}

void Commands::CommandReplySuccess() {
  msgLog = "Command Success.";
  msgLogger->MessageLogging(Error, msgLog);
  bytes = MakeUiFeedback(241, 0, 0);
  DevForwardMessageToUi();
}

void Commands::ForwardMessage() {
  std::vector<QTcpSocket*> targets;
  if (messageMap["floorID"] == 0 && messageMap["groupID"] == 0 && messageMap["roomID"] == 0) {
    // To  all devices
    for (auto iter : deviceMap) {
      targets.push_back(iter.first);
    }
  } else if (messageMap["floorID"] == 0 && messageMap["groupID"] == 0) {
    // To selected room type, all floor, all group
    for (auto iter : deviceMap) {
      if (messageMap["roomID"] == (iter.second)->GetRoomID()) {
        targets.push_back(iter.first);
      }
    }
  } else if (messageMap["floorID"] == 0 && messageMap["roomID"] == 0) {
    // To all floors, all rooms, selected group
    for (auto iter : deviceMap) {
      if (messageMap["groupID"] == (iter.second)->GetGroupID()) {
        targets.push_back(iter.first);
      }
    }
  } else if (messageMap["groupID"] == 0 && messageMap["roomID"] == 0) {
    // To selected floor, all rooms, all groups
    for (auto iter : deviceMap) {
      if (messageMap["floorID"] == (iter.second)->GetFloorID()) {
        targets.push_back(iter.first);
      }
    }
  } else if (messageMap["groupID"] == 0) {
    // To all groups, selected room, selected floor
    for (auto iter : deviceMap) {
      if (messageMap["floorID"] == (iter.second)->GetFloorID() && messageMap["roomID"] == (iter.second)->GetRoomID()) {
        targets.push_back(iter.first);
      }
    }
  } else if (messageMap["roomID"] == 0) {
    // To all rooms, selected floor, selected group
    for (auto iter : deviceMap) {
      if (messageMap["floorID"] == (iter.second)->GetFloorID() && messageMap["groupID"] == (iter.second)->GetGroupID()) {
        targets.push_back(iter.first);
      }
    }
  } else if (messageMap["floorID"] == 0) {
    // To all floors, selected room, selected group
    for (auto iter : deviceMap) {
      if (messageMap["roomID"] == (iter.second)->GetRoomID() && messageMap["groupID"] == (iter.second)->GetGroupID()) {
        targets.push_back(iter.first);
      }
    }
  } else {
    for (auto iter : deviceMap) {
      // To one device
      if ((iter.second)->GetDeviceIDHigh() == messageMap["targetIDHigh"] && (iter.second)->GetDeviceIDLow() == messageMap["targetIDLow"]) {
        targets.push_back(iter.first);
      }
    }
  }

  if (targets.empty()) {
    CommandReplyError();
  } else {
    // Send
    for (auto socket : targets) {
      //if (messageMap["cmdID"] == 246 || messageMap["cmdID"] == 1 || messageMap["cmdID"] == 2 || messageMap["cmdID"] == 3 ||
      //  messageMap["cmdID"] == 4 || messageMap["cmdID"] == 5) {
      //  if (messageMap["targetIDHigh"] == 255 && messageMap["targetIDLow"] == 253) { continue; }
      //}
      socket->write(msgConvert->BytesToQBytes(bytes) + '\n');
      msgLog = "Forwarding message to: " + msgConvert->QStringToString((socket->peerAddress()).toString()) + ". ";
      msgLog += "Sent: ";
      for (auto i : bytes) {
        msgLog += msgConvert->ByteToString(i) + " ";
      }
      msgLogger->MessageLogging(Log, msgLog);
    }
  }
}

void Commands::DevForwardMessageToUi() {
  for (auto iter : deviceMap) {
    if ((iter.second)->GetDeviceIDHigh() == 255 && (iter.second)->GetDeviceIDLow() == 253) {
      (iter.first)->write(msgConvert->BytesToQBytes(bytes) + '\n');
      msgLog = "Forwarding message to Ui. ";
      msgLog += "Sent: ";
      for (auto i : bytes) {
        msgLog += msgConvert->ByteToString(i) + " ";
      }
      msgLogger->MessageLogging(Log, msgLog);
    }
  }
}
