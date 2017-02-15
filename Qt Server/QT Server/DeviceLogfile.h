#ifndef DEVICELOGFILE_H
#define DEVICELOGFILE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <mutex>
#include <vector>
#include "MyTime.h"
#include "Device.h"

typedef unsigned char byte;

class DeviceLogfile {
private:

  std::string devicelogbuffer;
  std::string devicelogfilename;
  std::ofstream devicelogfile;
  MyTime* LocalTimer;
//  bool logging;
  std::mutex logmutex;
  int numberofids = 7;

public:
  DeviceLogfile();
  void device_log_buffer(std::string devicelogbuffer);
//  bool get_logging_status();
  std::vector<Device> get_devices_vector();
  Device get_device(std::string);
  std::string get_devicelogfilename();
};

#endif 
