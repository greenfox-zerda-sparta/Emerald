#include "Device.h"


Device::Device()
{
  
}

Device::~Device()
{
  
}

Device::Device(byte _deviceIDHigh, byte _deviceIDLow, std::string _IP)
{
  
}

//deviceIDHigh", "deviceIDLow", "groupID", "homeID", "floorID", "roomID", "cmdID" }

void Device::set_deviceIDHigh(byte)
{
 
}

void Device::set_deviceIDLow()
{
  
}

unsigned char Device::get_deviceIDHigh()
{
  return deviceIDHigh;
}

unsigned char Device::get_deviceIDLow()
{
  return deviceIDLow;
}

void Device::set_groupID()
{
  
}


byte Device::get_groupID()
{
  return groupID;
}

void Device::connect_Device()
{
  
}

void Device::disconnect_Device()
{
  
}

void Device::run_Main_Function(byte mainFunctionCommand)
{
  std::string deviceMainStatus;

  if (mainFunctionCommand == 0)
  {
    deviceMainStatus = "OFF";
  } else
  {
    deviceMainStatus = "ON";
  }

  send_Device_Main_Status_Message(deviceMainStatus);

}

std::string Device::send_Device_Main_Status_Message(std::string device_Main_Status)
{
  return device_Main_Status;
}

void Device::run_Second_Function(byte secondFunctionCommand)
{

}

void Device::run_Third_Function(byte thirdFunctionCommand)
{

}