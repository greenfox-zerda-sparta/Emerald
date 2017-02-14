#ifndef UI_H
#define UI_H

#include "Device.h"
#include "MyTime.h"
#include "Logfile.h"

class UI : public Device {
private:
  Logfile* mydevicelogfile;
  std::string devicelogbuffer;
  MyTime* LocalTimer;
  bool isdeviceworking;
public:
  UI(IDs _ids, std::string _IP);
  ~UI();
};


#endif
