#ifndef SUBDEVICE_H
#define SUBDEVICE_H

#include "Device.h"
#include "DeviceLogfile.h"
#include "MessageConverter.h"

class SubDevice : public Device {
  private:
    DeviceLogfile* myDeviceLogfile;
    std::string deviceLogBuffer;
    bool isDeviceWorking;
  public:
    SubDevice(IDs _ids, std::string _IP, bool _isDeviceWorking);
    SubDevice(std::map<std::string, byte>& messageMap, std::string _IP, bool _isDeviceWorking = true);
    ~SubDevice();
};

#endif
