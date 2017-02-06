#include "Device.h"


Device::Device()
{
  
}

Device::~Device()
{
  
}

Device::Device(unsigned char deviceIDHigh, unsigned char devideIDLow)
{
  
}

void Device::set_deviceIDHigh()
{
  deviceIDHigh = deviceCommandMap.at("DeviceIDHigh");
}

void Device::set_deviceIDLow()
{
  deviceIDLow = deviceCommandMap.at("DeviceIDLow");
}

unsigned char Device::get_deviceIDHigh()
{
  return deviceIDHigh;
}

unsigned char Device::get_deviceIDLow()
{
  return deviceIDLow;
}

void Device::connect_Device()
{
  
}

void Device::disconnect_Device()
{
  
}

void Device::run_Main_Function(unsigned char mainFunctionCommand)
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

void Device::run_Second_Function(unsigned char secondFunctionCommand)
{

}

void Device::run_Third_Function(unsigned char thirdFunctionCommand)
{

}