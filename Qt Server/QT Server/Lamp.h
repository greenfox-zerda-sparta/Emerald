#ifndef LAMP_H
#define LAMP_H

#include "Device.h"

class Lamp : public Device {
private:

public:
//  Lamp(byte _deviceIDHigh, byte _deviceIDLow, byte _homeID, byte _floorID, byte _roomID, byte _cmdID, std::string _IP);
  Lamp(IDs ids, std::string _IP);
};


#endif
