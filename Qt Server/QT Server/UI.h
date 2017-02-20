#ifndef UI_H
#define UI_H

#include "Device.h"
#include "DeviceLogfile.h"
#include "MessageConverter.h"

class UI : public Device {
  private:
    DeviceLogfile* mydevicelogfile;
    std::string devicelogbuffer;
    bool isdeviceworking;
  public:
    UI(IDs _ids, std::string _IP, bool isdeviceworking);
    ~UI();
};

#endif
