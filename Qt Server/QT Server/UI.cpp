#include "UI.h"

UI::UI(byte _deviceIDHigh, byte _deviceIDLow, byte _homeID, byte _floorID, byte _roomID, byte _cmdID, std::string _IP)
: Device(_deviceIDHigh, _deviceIDLow, _homeID, _floorID, _roomID, _cmdID, _IP){
  deviceIDHigh = 255;
  deviceIDLow = 253;
  groupID = 254;
  homeID = 255;
  floorID = 255;
  roomID = 255;
}
