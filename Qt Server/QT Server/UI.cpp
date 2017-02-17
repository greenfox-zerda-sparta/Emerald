#include "UI.h"

UI::UI(IDs _ids, std::string _IP, bool isdeviceworking) : Device(_ids, _IP, isdeviceworking) {
  mydevicelogfile = new DeviceLogfile;
  mydevicelogfile->DeviceLogging(
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
