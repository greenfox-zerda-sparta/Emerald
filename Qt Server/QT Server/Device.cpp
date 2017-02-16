#include "Device.h"

Device::Device(){}

Device::Device(IDs _ids, std::string _IP, bool _isdeviceworking) {
  deviceIDHigh = _ids.deviceIDHigh;
  deviceIDLow = _ids.deviceIDLow;
  homeID = _ids.homeID;
  floorID = _ids.floorID;
  roomID = _ids.roomID;
  groupID = _ids.groupID;
  IP = _IP;
  isdeviceworking = _isdeviceworking;
}

Device::Device(std::map<std::string, byte> messageMap, std::string _IP, bool _isdeviceworking) {
  deviceIDHigh = messageMap["deviceIDHigh"];
  deviceIDLow = messageMap["deviceIDLow"];
  groupID = messageMap["groupID"];
  homeID = messageMap["homeID"];
  floorID = messageMap["floorID"];
  roomID = messageMap["roomID"];
  IP = _IP;
  isdeviceworking = _isdeviceworking;
}

byte Device::get_deviceIDHigh() {
  return deviceIDHigh;
}

byte Device::get_deviceIDLow() {
  return deviceIDLow;
}

byte Device::get_groupID() {
  return groupID;
}

byte Device::get_homeID() {
  return homeID;
}

byte Device::get_floorID() {
  return floorID;
}

byte Device::get_roomID() {
  return roomID;
}

std::string Device::get_IP() {
  return IP;
}
