#include "UI.h"

UI::UI(IDs _ids, std::string _IP)
: Device(_ids, _IP, isdeviceworking){
  deviceIDHigh = 255;
  deviceIDLow = 253;
  groupID = 254;
  homeID = 255;
  floorID = 255;
  roomID = 255;

  bool isdeviceworking = true;
  mydevicelogfile = new DeviceLogfile;

  //  struct IDs {
  //    byte deviceIDHigh;
  //    byte deviceIDLow;
  //    byte groupID;
  //    byte homeID;
  //    byte floorID;
  //    byte roomID;
  //  };
  mydevicelogfile->device_log_buffer(
    toString(int(get_deviceIDHigh())) + " " +
    toString(int(get_deviceIDLow())) + " " +
    toString(int(get_groupID())) + " " +
    toString(int(get_homeID())) + " " +
    toString(int(get_floorID())) + " " +
    toString(int(get_roomID())) + " " +
    _IP + " " + 
    toString(isdeviceworking));
}

UI::~UI() {
  delete mydevicelogfile;
}
