#ifndef SUBDEVICE_H
#define SUBDEVICE_H
#include "MessageConverter.h"
#include "Device.h"
#include "DeviceLogfile.h"

class SubDevice : public Device {
private:
  DeviceLogfile* mydevicelogfile;
  std::string devicelogbuffer;
  bool isdeviceworking;
public:
  SubDevice(IDs _ids, std::string _IP, bool isdeviceworking);
  SubDevice(std::map<std::string, byte> messageMap, std::string _IP, bool _isdeviceworking = true);
  ~SubDevice();
};

#endif
