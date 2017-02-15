#include "Logfile.h"

Logfile::Logfile() {
  messagelogbuffer = "";
 // std::ofstream logfile;
  messagelogfilename = "Smart_Home_" + LocalTimer->GetTimeFileFormat() + ".txt";
  logging = true;
//  std::ofstream devicelog;
  devicelogfilename = "Smart_Home_Device_Log.txt";
}


void Logfile::message_log_buffer(LogLevel _loglevel, std::string messagelogbuffer) {

  const static char* LogLevelStr[] = { "UI", "Device", "Warning", "Error" };

  logmutex.lock();
  if (logging) {
    messagelogfile.open(messagelogfilename.c_str(), std::ios::app);
    messagelogfile << "[" << LogLevelStr[_loglevel] << "] " << messagelogbuffer << std::endl;
    messagelogfile.close();
  }
  logmutex.unlock();
}

void Logfile::device_log_buffer(std::string devicelogbuffer) { // ide kell az egesz vektor argumentkent
 // std::string devicelogbuffer;

  logmutex.lock();
//  if (logging) {
    devicelogfile.open(devicelogfilename.c_str(), std::ios::trunc);
    devicelogfile << devicelogbuffer << std::endl;
    devicelogfile.close();
//  }
  logmutex.unlock();
}

bool Logfile::get_logging_status() {
  return logging;
}

Device Logfile::get_device(std::string buffer) {
  std::vector<std::string> device_stuff;
  bool isdeviceworking;
  size_t position = buffer.find(" ");
   while (position != std::string::npos) {
    std::string content = buffer.substr(0, position);  
    buffer = buffer.substr(position+1);
    device_stuff.push_back(content);
    position = buffer.find(" ");
    if(position == std::string::npos) {
      device_stuff.push_back(buffer);
    }
  }
  byte deviceIDHigh = byte(stoi(device_stuff[0]));
  byte deviceIDLow = byte(stoi(device_stuff[1]));
  byte homeID = byte(stoi(device_stuff[2]));
  byte floorID = byte(stoi(device_stuff[3]));
  byte roomID = byte(stoi(device_stuff[4]));
  byte groupID = byte(stoi(device_stuff[5]));
  std::string IP = device_stuff[6];
  if (device_stuff[7]== "1") {
    isdeviceworking = true;
  } else {
    isdeviceworking = false;
  }
  Device returnDevice(IDs{ deviceIDHigh, deviceIDLow, homeID, floorID, roomID, groupID }, IP, isdeviceworking);
  return returnDevice;
}

std::vector<Device> Logfile::get_devices_vector() {
  std::vector<Device> devices;
  std::ifstream devicelogfile(devicelogfilename.c_str());
  std::string buffer;
  while (getline(devicelogfile, buffer)) {
    if(buffer.length() < 3) break;
    devices.push_back(get_device(buffer));
  }
  devicelogfile.close();
  return devices;
}

std::string Logfile::get_devicelogfilename() {
  return devicelogfilename;
}
