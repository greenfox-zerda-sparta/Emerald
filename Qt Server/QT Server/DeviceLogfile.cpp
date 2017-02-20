#include "DeviceLogfile.h"

DeviceLogfile::DeviceLogfile() {
  devicelogfilename = "Smart_Home_Device_Log.txt";
}

void DeviceLogfile::DeviceLogging(std::string devicelogbuffer) {
  logmutex.lock();
  devicelogfile.open(devicelogfilename.c_str(), std::ios::trunc);
  devicelogfile << devicelogbuffer << std::endl;
  devicelogfile.close();
  logmutex.unlock();
}

Device* DeviceLogfile::get_device(std::string buffer) {
  std::vector<std::string> device_stuff;
  bool isdeviceworking;
  size_t position = buffer.find(" ");
  while (position != std::string::npos) {
    std::string content = buffer.substr(0, position);
    buffer = buffer.substr(position + 1);
    device_stuff.push_back(content);
    position = buffer.find(" ");
    if (position == std::string::npos) {
      device_stuff.push_back(buffer);
    }
  }
  byte deviceIDHigh = byte(stoi(device_stuff[0]));
  byte deviceIDLow = byte(stoi(device_stuff[1]));
  byte groupID = byte(stoi(device_stuff[2]));
  byte homeID = byte(stoi(device_stuff[3]));
  byte floorID = byte(stoi(device_stuff[4]));
  byte roomID = byte(stoi(device_stuff[5]));
  std::string IP = device_stuff[6];
  if (device_stuff[7] == "1") {
    isdeviceworking = true;
  } else {
    isdeviceworking = false;
  }
  Device* returnDevice = new Device(IDs{deviceIDHigh, deviceIDLow, groupID, homeID, floorID, roomID}, IP, isdeviceworking);
  return returnDevice;
}

std::vector<Device*> DeviceLogfile::get_devices_vector() {
  std::vector<Device*> devices;
  std::ifstream devicelogfile(devicelogfilename.c_str());
  std::string buffer;
  while (getline(devicelogfile, buffer)) {
    if (buffer.length() < 3) {
      continue;
    }
    devices.push_back(get_device(buffer));
  }
  devicelogfile.close();
  return devices;
}
