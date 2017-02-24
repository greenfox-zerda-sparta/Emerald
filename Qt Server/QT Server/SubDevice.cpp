#include "SubDevice.h"

SubDevice::SubDevice(IDs _ids, std::string _IP, bool isdeviceworking) : Device(_ids, _IP, isdeviceworking) {
  myDeviceLogfile = new DeviceLogfile;
  myDeviceLogfile->DeviceLogging(
    ToString(int(GetDeviceIDHigh())) + " " +
    ToString(int(GetDeviceIDLow())) + " " +
    ToString(int(GetGroupID())) + " " +
    ToString(int(GetHomeID())) + " " +
    ToString(int(GetFloorID())) + " " +
    ToString(int(GetRoomID())) + " " +
    _IP + " " +
    ToString(isdeviceworking));
}

SubDevice::SubDevice(std::map<std::string, byte>& messageMap, std::string _IP, bool _isdeviceworking) {
  deviceIDHigh = messageMap["deviceIDHigh"];
  deviceIDHigh = messageMap["deviceIDLow"];
  homeID = messageMap["homeID"];
  floorID = messageMap["floorID"];
  roomID = messageMap["roomID"];
  groupID = messageMap["groupID"];
  IP = _IP;
  isDeviceWorking = _isdeviceworking;
  /*myDeviceLogfile = new DeviceLogfile;
    myDeviceLogfile->DeviceLogging(
    ToString(int(GetDeviceIDHigh())) + " " +
    ToString(int(GetDeviceIDLow())) + " " +
    ToString(int(GetGroupID())) + " " +
    ToString(int(GetHomeID())) + " " +
    ToString(int(GetFloorID())) + " " +
    ToString(int(GetRoomID())) + " " +
    _IP + " " +
    ToString(isDeviceWorking));*/
}

SubDevice::~SubDevice() {
  delete myDeviceLogfile;
}
