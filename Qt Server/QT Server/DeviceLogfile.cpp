#include "DeviceLogfile.h"
#include <iostream>

DeviceLogfile::DeviceLogfile() {
  deviceLogFilename = "Smart_Home_Device_Log.txt";
}

void DeviceLogfile::DeviceLogging(std::string devicelogbuffer) {
  logMutex.lock();
  deviceLogfile.open(deviceLogFilename, std::ios::trunc);
  deviceLogfile << devicelogbuffer << std::endl;
  deviceLogfile.close();
  logMutex.unlock();
}

std::shared_ptr<Device> DeviceLogfile::GetDevice(std::string buffer) {
  std::vector<std::string> deviceStuff;
  bool isDeviceWorking;
  size_t position = buffer.find(" ");
  while (position != std::string::npos) {
    std::string content = buffer.substr(0, position);
    buffer = buffer.substr(position + 1);
    deviceStuff.push_back(content);
    position = buffer.find(" ");
    if (position == std::string::npos) {
      deviceStuff.push_back(buffer);
    }
  }
  byte deviceIDHigh = byte(stoi(deviceStuff[0]));
  byte deviceIDLow = byte(stoi(deviceStuff[1]));
  byte groupID = byte(stoi(deviceStuff[2]));
  byte homeID = byte(stoi(deviceStuff[3]));
  byte floorID = byte(stoi(deviceStuff[4]));
  byte roomID = byte(stoi(deviceStuff[5]));
  std::string IP = deviceStuff[6];
  if (deviceStuff[7] == "1") {
    isDeviceWorking = true;
  } else {
    isDeviceWorking = false;
  }
  std::shared_ptr<Device> returnDevice = std::shared_ptr<Device>(new Device(IDs{deviceIDHigh, deviceIDLow, groupID, homeID, floorID, roomID}, IP, isDeviceWorking));
  return returnDevice;
}

std::vector<std::shared_ptr<Device> > DeviceLogfile::GetDevicesVector() {
  std::vector<std::shared_ptr<Device>> devices;
  deviceLog.open(deviceLogFilename);
  std::string buffer;
  while (getline(deviceLog, buffer)) {
    if (buffer.length() < 3) {
      continue;
    }
    devices.push_back(GetDevice(buffer));
  }
  deviceLog.close();
  return devices;
}

void DeviceLogfile::ClearLogfile() {
  std::ofstream ofs;
  ofs.open(deviceLogFilename, std::ofstream::out | std::ofstream::trunc);
  ofs.close();
}
