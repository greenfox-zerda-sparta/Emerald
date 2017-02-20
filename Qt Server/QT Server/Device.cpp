#include "Device.h"

Device::Device() {}

Device::Device(IDs _ids, std::string _IP, bool _isdeviceworking) {
  deviceIDHigh = _ids.deviceIDHigh;
  deviceIDLow = _ids.deviceIDLow;
  homeID = _ids.homeID;
  floorID = _ids.floorID;
  roomID = _ids.roomID;
  groupID = _ids.groupID;
  IP = _IP;
  isDeviceWorking = _isdeviceworking;
}

Device::Device(std::map<std::string, byte> messageMap, std::string _IP, bool _isdeviceworking) {
  deviceIDHigh = messageMap["deviceIDHigh"];
  deviceIDLow = messageMap["deviceIDLow"];
  groupID = messageMap["groupID"];
  homeID = messageMap["homeID"];
  floorID = messageMap["floorID"];
  roomID = messageMap["roomID"];
  IP = _IP;
  isDeviceWorking = _isdeviceworking;
}

byte Device::GetDeviceIDHigh() {
  return deviceIDHigh;
}

byte Device::GetDeviceIDLow() {
  return deviceIDLow;
}

byte Device::GetGroupID() {
  return groupID;
}

byte Device::GetHomeID() {
  return homeID;
}

byte Device::GetFloorID() {
  return floorID;
}

byte Device::GetRoomID() {
  return roomID;
}

std::string Device::GetIP() {
  return IP;
}

byte Device::IsWorking() {
  return isDeviceWorking ? 1 : 0;
}