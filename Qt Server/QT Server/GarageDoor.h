#ifndef GARAGEDOOR_H
#define GARAGEDOOR_H

#include "Device.h"

class GarageDoor : public Device {
private:

public:
  GarageDoor(byte _deviceIDHigh, byte _deviceIDLow, byte _homeID, byte _floorID, byte _roomID, byte _cmdID, std::string _IP);
};


#endif