#include "UI.h"
#include "Logfile.h"

UI::UI(IDs _ids, std::string _IP)
: Device(_ids, _IP, isdeviceworking){
  deviceIDHigh = 255;
  deviceIDLow = 253;
  groupID = 254;
  homeID = 255;
  floorID = 255;
  roomID = 255;

  bool isdeviceworking = true;
  mydevicelogfile = new Logfile;

  mydevicelogfile->device_log_buffer(
    toString(int(get_homeID())) + " " +
    toString(int(get_floorID())) + " " +
    toString(int(get_roomID())) + " " +
    toString(int(get_groupID())) + " " +
    toString(int(get_deviceIDHigh())) + " " +
    toString(int(get_deviceIDLow())) + " " +
    _IP + toString(isdeviceworking) + "\n");
}

UI::~UI() {
  delete mydevicelogfile;
}
