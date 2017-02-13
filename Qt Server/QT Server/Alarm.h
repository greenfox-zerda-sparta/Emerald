#ifndef ALARM_H
#define ALARM_H

#include "Device.h"

class Alarm : public Device {
private:

public:
  Alarm(IDs _ids, std::string _IP);
};


#endif