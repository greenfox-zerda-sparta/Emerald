#ifndef HEATING_H
#define HEATING_H

#include "Device.h"

class Heating : public Device {
private:

public:
  Heating(IDs _ids, std::string _IP);
};


#endif
