#ifndef DEVICE_H
#define DEVICE_H

#include <unordered_map>

typedef unsigned char byte;

class Device
{
private:
  byte deviceIDHigh;
  byte deviceIDLow;
  byte groupID;
  std::unordered_map<std::string, byte> deviceCommandMap;
  byte mainFunctionCommand;
  byte secondFunctionCommand;
  byte thirdFunctionCommand;
  bool isSecondFunction;
  bool isThirdFunction;
public:
  Device();
  Device(byte, byte, std::string);
  ~Device();
  void set_deviceIDHigh();
  void set_deviceIDLow();
  byte get_deviceIDHigh();
  byte get_deviceIDLow();
  void connect_Device();
  void disconnect_Device();
  virtual void run_Main_Function(byte) = 0;
  virtual void run_Second_Function(byte);
  virtual void run_Third_Function(byte);
  std::string send_Device_Main_Status_Message(std::string device_Main_Status);

};


#endif
