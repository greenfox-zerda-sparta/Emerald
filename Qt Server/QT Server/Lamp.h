#ifndef LAMP_H
#define LAMP_H

#include <unordered_map>
#include "Device.h"

typedef unsigned char byte;

class Lamp : public Device
{
private:
  std::unordered_map<std::string, byte> deviceCommandMap;
  byte mainFunctionCommand;
  byte secondFunctionCommand;
  byte thirdFunctionCommand;
  bool isSecondFunction;
  bool isThirdFunction;
public:
  Lamp();
  Lamp(byte, byte);
  ~Lamp();
  void set_groupID();
  byte get_groupID();

};


#endif
