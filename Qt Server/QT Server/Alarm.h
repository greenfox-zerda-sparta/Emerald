#ifndef ALARM_H
#define ALARM_H

#include "Device.h"

class Alarm : public Device {
private:

public:
  Alarm(byte _deviceIDHigh, byte _deviceIDLow, byte _homeID, byte _floorID, byte _roomID, byte _cmdID, std::string _IP);
};


#endif