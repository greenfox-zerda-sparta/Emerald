#ifndef DEVICE_H
#define DEVICE_H
#include <string>
#include <map>

typedef unsigned char byte;

struct IDs {
  byte deviceIDHigh;
  byte deviceIDLow;
  byte groupID;
  byte homeID;
  byte floorID;
  byte roomID;
};

class Device {
protected:
  byte deviceIDHigh;
  byte deviceIDLow;
  byte groupID;
  byte homeID;
  byte floorID;
  byte roomID;
  std::string IP;
  bool isdeviceworking;
public:
  Device();
  Device(IDs ids, std::string _IP, bool _isdeviceworking = true);
  Device(std::map<std::string, byte> messageMap, std::string _IP, bool _isdeviceworking = true);
  byte get_deviceIDHigh();
  byte get_deviceIDLow();
  byte get_groupID();
  byte get_homeID();
  byte get_floorID();
  byte get_roomID();
  std::string get_IP();
};


#endif
