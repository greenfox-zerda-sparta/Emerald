#include "Device.h"


Device::Device(byte _deviceIDHigh, byte _deviceIDLow, byte _homeID, byte _floorID, byte _roomID, byte _cmdID, std::string _IP) {
  deviceIDHigh = _deviceIDHigh;
  deviceIDLow = _deviceIDLow;
  homeID = _homeID;
  floorID = _floorID;
  roomID = _roomID;
  cmdID = _cmdID;
  IP = _IP;
}

unsigned char Device::get_deviceIDHigh() {
  return deviceIDHigh;
}

unsigned char Device::get_deviceIDLow() {
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

byte Device::get_cmdID() {
  return cmdID;
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
