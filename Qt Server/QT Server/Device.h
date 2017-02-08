#ifndef DEVICE_H
#define DEVICE_H
#include <string>

typedef unsigned char byte;

class Device
{
protected:
  byte deviceIDHigh;
  byte deviceIDLow;
  byte groupID;
  byte homeID;
  byte floorID;
  byte roomID;
  byte cmdID;
  std::string IP;

public:
  Device(byte _deviceIDHigh, byte _deviceIDLow, byte _homeID, byte _floorID, byte _roomID, byte _cmdID, std::string _IP);
  byte get_deviceIDHigh();
  byte get_deviceIDLow();
  byte get_groupID();
  byte get_homeID();
  byte get_floorID();
  byte get_roomID();
  byte get_cmdID();
  std::string get_IP();
  void connect_Device();
  void disconnect_Device();
  std::string send_Device_Main_Status_Message(std::string device_Main_Status);
};


#endif
