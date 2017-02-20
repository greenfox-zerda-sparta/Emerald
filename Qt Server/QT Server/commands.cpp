#include "commands.h"

Commands::Commands(std::vector<Device*>& _addedDevices) : addedDevs(_addedDevices) {
  deviceLog = new DeviceLogfile;
  msgConvert = new MessageConverter;
  ptr_resetServer = &Commands::ResetServer;
  ptr_restartServer = &Commands::RestartServer;
  ptr_stopServer = &Commands::StopServer;
  ptr_addDevice = &Commands::AddDevice;
  ptr_removeDevice = &Commands::RemoveDevice;
  ptr_getStatusReport = &Commands::GetStatusReport;
  ptr_setData = &Commands::SetData;
  ptr_forwardMessage = &Commands::ForwardMessage;
  cmdMap[239] = ptr_setData;
  cmdMap[246] = ptr_getStatusReport;
  cmdMap[247] = ptr_addDevice;
  cmdMap[248] = ptr_removeDevice;
  cmdMap[253] = ptr_resetServer;
  cmdMap[254] = ptr_restartServer;
  cmdMap[255] = ptr_stopServer;
  cmdMap[0] = ptr_forwardMessage;
  cmdMap[1] = ptr_forwardMessage;
  cmdMap[2] = ptr_forwardMessage;
  cmdMap[3] = ptr_forwardMessage;
  cmdMap[4] = ptr_forwardMessage;
  cmdMap[5] = ptr_forwardMessage;
}

Commands::~Commands() {
  delete msgConvert;
  delete deviceLog;
}

void Commands::SetMessageMap(std::map<std::string, byte>& _messageMap) {
  this->messageMap = _messageMap;
}

void Commands::SetAddedDevices(std::vector<Device*>& _addedDevices) {
  this->addedDevs = _addedDevices;
}

void Commands::SetDeviceMap(std::map<QTcpSocket*, Device*>& _deviceMap) {
  this->deviceMap = _deviceMap;
}

void Commands::SetBytes(std::vector<byte>& _bytes) {
  this->bytes = _bytes;
}

void Commands::RunCommand() {
  if (cmdMap.count(messageMap["cmdID"]) < 1) {
    std::cerr << "INVALID COMMAND" << std::endl;
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
    std::cout << "RESETTING SERVER" << std::endl; // reset server;  delete all devices
  } else {
    std::cerr << "Invalid command: target must be the server." << std::endl;
  }
}

void Commands::RestartServer() {
  if (IsServerCommand()) {
    std::cout << "RESTARTING SERVER" << std::endl; // restart server;
  } else {
    std::cerr << "Invalid command: target must be the server." << std::endl;
  }
}

void Commands::StopServer() {
  if (IsServerCommand()) {
    std::cout << "STOPPING SERVER" << std::endl; //stop server;
  } else {
    std::cerr << "Invalid command: target must be the server." << std::endl;
  }
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

std::string Commands::GetDeviceText(Device* dev) {
  return msgConvert->ByteToString(dev->GetDeviceIDHigh()) + " " +
         msgConvert->ByteToString(dev->GetDeviceIDLow()) + " " +
         msgConvert->ByteToString(dev->GetGroupID()) + " " +
         msgConvert->ByteToString(dev->GetHomeID()) + " " +
         msgConvert->ByteToString(dev->GetFloorID()) + " " +
         msgConvert->ByteToString(dev->GetRoomID()) + " " +
         ToString(dev->GetIP()) + " " +
         msgConvert->ByteToString(dev->IsWorking()) + "\n";
}

bool Commands::IsRoomForDevice() {
  return (IDHigh < 252 && IDLow < 252); // 
}

void Commands::LogDeviceList() {
  for (Device* device : addedDevs) {
    deviceLogBuffer += GetDeviceText(device);
  }
  std::cout << deviceLogBuffer;
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
        std::cout << "ADDING DEVICE" << std::endl;
        Device* newDevice = new Device(messageMap, IP);
        addedDevs.push_back(newDevice);
        LogDeviceList();
        //log 2x: device log es normal log
      } else {
        std::cerr << "Warning: no more devices can be added." << std::endl;
      }
    }
  } else {
    std::cerr << "Invalid command." << std::endl;
  }
}

void Commands::RemoveDevice() {
  if (IsServerCommand()) {
    std::cout << "REMOVING DEVICE" << std::endl; //remove device;
    for (int i = 0; i < addedDevs.size(); i++) {
      if (addedDevs[i]->GetDeviceIDHigh() == messageMap["body1"] &&
          addedDevs[i]->GetDeviceIDLow() == messageMap["body2"]) {
        addedDevs.erase(addedDevs.begin() + i);
      }
    }
    LogDeviceList();
    // needs message log too. Removing from onlineDevices map?
  } else {
    std::cerr << "Invalid command: target must be the server." << std::endl;
  }
}

void Commands::GetStatusReport() {
  if (IsSenderUi()) {
    std::cout << "TO DEVICES/GETTING INFOS FROM DEVICES" << std::endl; //getting reports from devices;
  } else {
    std::cerr << "Invalid command: sender must be the User Interface." << std::endl;
  }
}

void Commands::SetData() {
  if (IsSenderUi()) {
    std::cout << "TO DEVICES/TO SET ID" << std::endl; //devices to set id;
  } else {
    std::cerr << "Invalid command: sender must be the User Interface." << std::endl;
  }
}

void Commands::ForwardMessage() {
  std::cout << "FORWARDING MESSAGE TO TARGET DEVICE" << std::endl; //forward message;
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
      if (messageMap["deviceIDHigh"] == (iter.second)->GetDeviceIDHigh() && messageMap["devideIDLow"] == (iter.second)->GetDeviceIDLow()) {
        targets.push_back(iter.first);
      }
    }
  }
  // Send
  for (auto socket : targets) {
    socket->write(msgConvert->BytesToQBytes(bytes) + '\n');
  }
}
