#include "SubDevice.h"
#include "MessageConverter.h"

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
    _IP + " " + 
    toString(isdeviceworking));
}

SubDevice::SubDevice(std::map<std::string, byte> messageMap, std::string _IP, bool _isdeviceworking) {
  homeID = messageMap["homeID"];
  floorID = messageMap["floorID"];
  roomID = messageMap["roomID"];
  groupID = messageMap["groupID"];
  IP = _IP;
  isdeviceworking = _isdeviceworking;
  mydevicelogfile = new Logfile;
  mydevicelogfile->device_log_buffer(
    toString(int(get_homeID())) + " " +
    toString(int(get_floorID())) + " " +
    toString(int(get_roomID())) + " " +
    toString(int(get_groupID())) + " " +
    toString(int(get_deviceIDHigh())) + " " +
    toString(int(get_deviceIDLow())) + " " +
    _IP + " " + 
    toString(isdeviceworking));
}

SubDevice::~SubDevice() {
  delete mydevicelogfile;
}
