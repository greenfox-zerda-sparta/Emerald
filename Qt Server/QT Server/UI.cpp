#include "UI.h"

UI::UI(IDs _ids, std::string _IP)
: Device(_ids, _IP){
  deviceIDHigh = 255;
  deviceIDLow = 253;
  groupID = 254;
  homeID = 255;
  floorID = 255;
  roomID = 255;
}

