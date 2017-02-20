#include "SubDevice.h"

SubDevice::SubDevice(IDs _ids, std::string _IP, bool isdeviceworking) : Device(_ids, _IP, isdeviceworking) {
  mydevicelogfile = new DeviceLogfile;
  mydevicelogfile->DeviceLogging(
    toString(int(get_deviceIDHigh())) + " " +
    toString(int(get_deviceIDLow())) + " " +
    toString(int(get_groupID())) + " " +
    toString(int(get_homeID())) + " " +
    toString(int(get_floorID())) + " " +
    toString(int(get_roomID())) + " " +
    _IP + " " +
    toString(isdeviceworking));
}

SubDevice::SubDevice(std::map<std::string, byte>& messageMap, std::string _IP, bool _isdeviceworking) {
  deviceIDHigh = messageMap["deviceIDHigh"];
  deviceIDHigh = messageMap["deviceIDLow"];
  homeID = messageMap["homeID"];
  floorID = messageMap["floorID"];
  roomID = messageMap["roomID"];
  groupID = messageMap["groupID"];
  IP = _IP;
  isdeviceworking = _isdeviceworking;
  /*mydevicelogfile = new DeviceLogfile;
    mydevicelogfile->DeviceLogging(
    toString(int(get_deviceIDHigh())) + " " +
    toString(int(get_deviceIDLow())) + " " +
    toString(int(get_groupID())) + " " +
    toString(int(get_homeID())) + " " +
    toString(int(get_floorID())) + " " +
    toString(int(get_roomID())) + " " +
    _IP + " " +
    toString(isdeviceworking));*/
}

SubDevice::~SubDevice() {
  delete mydevicelogfile;
}
