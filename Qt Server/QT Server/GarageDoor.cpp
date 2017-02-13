#include "GarageDoor.h"

GarageDoor::GarageDoor(IDs _ids, std::string _IP)
  : Device(_ids, _IP) {
  groupID = 6;
}