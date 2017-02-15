#ifndef SUBDEVICE_H
#define SUBDEVICE_H

#include "Device.h"
#include "Logfile.h"

class SubDevice : public Device {
private:
  Logfile* mydevicelogfile;
  std::string devicelogbuffer;
  MyTime* LocalTimer;
  bool isdeviceworking;
public:
  SubDevice(IDs _ids, std::string _IP);
  SubDevice(std::map<std::string, byte> messageMap, std::string _IP = "111.111.11.11", bool _isdeviceworking = true);
  ~SubDevice();
};


#endif