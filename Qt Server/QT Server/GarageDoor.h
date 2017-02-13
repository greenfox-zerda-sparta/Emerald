#ifndef GARAGEDOOR_H
#define GARAGEDOOR_H

#include "Device.h"

class GarageDoor : public Device {
private:

public:
  GarageDoor(IDs _ids, std::string _IP);
};


#endif