#ifndef UI_H
#define UI_H

#include "Device.h"

class UI : public Device {
private:

public:
  UI(byte _deviceIDHigh, byte _deviceIDLow, byte _homeID, byte _floorID, byte _roomID, byte _cmdID, std::string _IP);
};


#endif
