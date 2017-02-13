#include "Cooling.h"

Cooling::Cooling(IDs _ids, std::string _IP)
  : Device(_ids, _IP) {
  groupID = 4;
}