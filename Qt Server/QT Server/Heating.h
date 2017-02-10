#ifndef HEATING_H
#define HEATING_H

#include "Device.h"

class Heating : public Device {
private:

public:
  Heating(byte _deviceIDHigh, byte _deviceIDLow, byte _homeID, byte _floorID, byte _roomID, byte _cmdID, std::string _IP);
};


#endif
