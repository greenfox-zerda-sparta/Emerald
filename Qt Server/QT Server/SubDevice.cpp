#include "SubDevice.h"
#include "messageconverter.h"

SubDevice::SubDevice(IDs _ids, std::string _IP)
  : Device(_ids, _IP, isdeviceworking) {
  bool isdeviceworking = true;
  mydevicelogfile = new Logfile;

  mydevicelogfile->device_log_buffer(
    toString(int(get_homeID())) + " " +
    toString(int(get_floorID())) + " " +
    toString(int(get_roomID())) + " " +
    toString(int(get_groupID())) + " " +
    toString(int(get_deviceIDHigh())) + " " +
    toString(int(get_deviceIDLow())) + " " +
    _IP + toString(isdeviceworking) + "\n");
}

SubDevice::~SubDevice() {
  delete mydevicelogfile;
}
