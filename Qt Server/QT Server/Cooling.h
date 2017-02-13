#ifndef COOLING_H
#define COOLING_H

#include "Device.h"

class Cooling : public Device {
private:

public:
  Cooling(IDs _ids, std::string _IP);
};


#endif
