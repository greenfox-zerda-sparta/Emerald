#include "UI.h"

UI::UI(IDs _ids, std::string _IP, bool isDeviceWorking) : Device(_ids, _IP, isDeviceWorking) {
  myDeviceLogfile = new DeviceLogfile;
  myDeviceLogfile->DeviceLogging(
    ToString(int(GetDeviceIDHigh())) + " " +
    ToString(int(GetDeviceIDLow())) + " " +
    ToString(int(GetGroupID())) + " " +
    ToString(int(GetHomeID())) + " " +
    ToString(int(GetFloorID())) + " " +
    ToString(int(GetRoomID())) + " " +
    _IP + " " +
    ToString(isDeviceWorking));
}

UI::~UI() {
  delete myDeviceLogfile;
}
