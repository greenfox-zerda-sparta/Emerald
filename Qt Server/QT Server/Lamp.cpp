#include "Lamp.h"


Lamp::Lamp(byte _deviceIDHigh, byte _deviceIDLow, byte _homeID, byte _floorID, byte _roomID, byte _cmdID, std::string _IP) 
: Device(_deviceIDHigh, _deviceIDLow, _homeID, _floorID, _roomID, _cmdID, _IP) {
  groupID = '1';
}

