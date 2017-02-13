#include "Blinds.h"

Blinds::Blinds(IDs _ids, std::string _IP)
  : Device(_ids, _IP) {
  groupID = 2;
}