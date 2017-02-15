#include "Device.h"

Device::Device()
{
  
}

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
  homeID = messageMap["homeID"];
  floorID = messageMap["floorID"];
  roomID = messageMap["roomID"];
  groupID = messageMap["groupID"];
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

void Device::connect_Device() {
}

void Device::disconnect_Device() {
}

//
//void Device::run_Main_Function(byte mainFunctionCommand)
//{
//  std::string deviceMainStatus;
//
//  if (mainFunctionCommand == 0)
//  {
//    deviceMainStatus = "OFF";
//  } else
//  {
//    deviceMainStatus = "ON";
//  }
//
//  send_Device_Main_Status_Message(deviceMainStatus);
//
//}
//
//std::string Device::send_Device_Main_Status_Message(std::string device_Main_Status)
//{
//  return device_Main_Status;
//}
