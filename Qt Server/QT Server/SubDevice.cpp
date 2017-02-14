#include "SubDevice.h"

SubDevice::SubDevice(IDs _ids, std::string _IP)
  : Device(_ids, _IP) {
  mydevicelogfile = new Logfile;

//  mydevicelogfile->device_log_buffer(LocalTimer->GetTimeFileFormat() + _ids + _IP);
}

SubDevice::~SubDevice() {
  delete mydevicelogfile;
}