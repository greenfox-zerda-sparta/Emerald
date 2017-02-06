#ifndef DEVICE_H
#define DEVICE_H

#include <unordered_map>

class Device
{
private:
  unsigned char deviceIDHigh;
  unsigned char deviceIDLow;
  std::unordered_map<std::string, unsigned char> deviceCommandMap;
  unsigned char mainFunctionCommand;
  unsigned char secondFunctionCommand;
  unsigned char thirdFunctionCommand;
  bool isSecondFunction;
  bool isThirdFunction;
public:
  Device();
  Device(unsigned char, unsigned char);
  ~Device();
  void set_deviceIDHigh();
  void set_deviceIDLow();
  unsigned char get_deviceIDHigh();
  unsigned char get_deviceIDLow();
  void connect_Device();
  void disconnect_Device();
  virtual void run_Main_Function(unsigned char) = 0;
  virtual void run_Second_Function(unsigned char);
  virtual void run_Third_Function(unsigned char);
  std::string send_Device_Main_Status_Message(std::string device_Main_Status);

};


#endif
