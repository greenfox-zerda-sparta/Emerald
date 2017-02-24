#ifndef UI_H
#define UI_H

#include "Device.h"
#include "DeviceLogfile.h"
#include "MessageConverter.h"

class UI : public Device {
  private:
    DeviceLogfile* myDeviceLogfile;
    std::string deviceLogBuffer;
    bool isDeviceWorking;
  public:
    UI(IDs _ids, std::string _IP, bool _isDeviceWorking);
    ~UI();
};

#endif
