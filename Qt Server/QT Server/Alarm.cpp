#include "Alarm.h"

Alarm::Alarm(IDs _ids, std::string _IP)
  : Device(_ids, _IP) {
  groupID = 5;
}
