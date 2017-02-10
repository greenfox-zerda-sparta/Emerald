#ifndef COOLING_H
#define COOLING_H

#include "Device.h"

class Cooling : public Device {
private:

public:
  Cooling(byte _deviceIDHigh, byte _deviceIDLow, byte _homeID, byte _floorID, byte _roomID, byte _cmdID, std::string _IP);
};


#endif
