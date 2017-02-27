#ifndef DEVICELOGFILE_H
#define DEVICELOGFILE_H

#include <fstream>
#include <mutex>
#include <memory>
#include <vector>
#include "Device.h"

typedef unsigned char byte;

class DeviceLogfile {
  private:
    std::string deviceLogBuffer;
    std::string deviceLogFilename;
    std::ofstream deviceLogfile;
    std::ifstream deviceLog;
    std::mutex logMutex;
  public:
    DeviceLogfile();
    void DeviceLogging(std::string devicelogbuffer);
    std::vector<std::shared_ptr<Device>> GetDevicesVector();
    std::shared_ptr<Device> GetDevice(std::string);
    void ClearLogfile();
};

#endif
