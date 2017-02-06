#ifndef LAMP_H
#define LAMP_H

#include <unordered_map>
#include "Device.h"

class Lamp : public Device
{
private:
  unsigned char groupID;
  std::unordered_map<std::string, unsigned char> deviceCommandMap;
  unsigned char mainFunctionCommand;
  unsigned char secondFunctionCommand;
  unsigned char thirdFunctionCommand;
  bool isSecondFunction;
  bool isThirdFunction;
public:
  Lamp();
  Lamp(unsigned char, unsigned char);
  ~Lamp();
  void set_groupID();
  unsigned char get_groupID();

};


#endif
