#ifndef DEVICELOGFILE_H
#define DEVICELOGFILE_H

#include <fstream>
#include <mutex>
#include <vector>
#include "Device.h"

typedef unsigned char byte;

class DeviceLogfile {
  private:
    std::string devicelogbuffer;
    std::string devicelogfilename;
    std::ofstream devicelogfile;
    std::mutex logmutex;

  public:
    DeviceLogfile();
    void DeviceLogging(std::string devicelogbuffer);
    std::vector<Device*> get_devices_vector();
    Device* get_device(std::string);
};

#endif
