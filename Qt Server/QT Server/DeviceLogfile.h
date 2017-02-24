#ifndef DEVICELOGFILE_H
#define DEVICELOGFILE_H

#include <fstream>
#include <mutex>
#include <vector>
#include "Device.h"

typedef unsigned char byte;

class DeviceLogfile {
  private:
    std::string deviceLogBuffer;
    std::string deviceLogFilename;
    std::ofstream deviceLogfile;
    std::mutex logMutex;
  public:
    DeviceLogfile();
    void DeviceLogging(std::string devicelogbuffer);
    std::vector<Device*> GetDevicesVector();
    Device* GetDevice(std::string);
    void ClearLogfile();
};

#endif
