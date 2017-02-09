#ifndef BLINDS_H
#define BLINDS_H

#include "Device.h"

class Blinds : public Device {
private:

public:
  Blinds(byte _deviceIDHigh, byte _deviceIDLow, byte _homeID, byte _floorID, byte _roomID, byte _cmdID, std::string _IP);
};


#endif
