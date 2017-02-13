#include "Heating.h"

Heating::Heating(IDs _ids, std::string _IP)
  : Device(_ids, _IP) {
  groupID = 3;
}