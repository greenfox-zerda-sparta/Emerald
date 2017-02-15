#ifndef COMMANDS_H
#define COMMANDS_H
#include <vector>
#include <memory>
#include <map>
#include <iostream>
#include "Device.h"
//#include "server.h"

typedef unsigned char byte;

class Commands {
public:
  Commands();
  void setMessageMap(std::map<std::string, byte>& messageMap);
  void setAddedDevices(std::vector<Device>* _addedDevices);
  void runCommand();
private:
  void resetServer();
  void restartServer();
  void stopServer();
  void addDevice();
  void removeDevice();
  void getStatusReport();
  void setData();
  void forwardMessage();
  std::map<std::string, byte> messageMap;
  std::map<byte, void(Commands::*)()> cmdMap;
  void(Commands::*ptr_resetServer)();
  void(Commands::*ptr_restartServer)();
  void(Commands::*ptr_stopServer)();
  void(Commands::*ptr_addDevice)();
  void(Commands::*ptr_removeDevice)();
  void(Commands::*ptr_getStatusReport)();
  void(Commands::*ptr_setData)();
  void(Commands::*ptr_forwardMessage)();
  bool isSenderUi();
  bool isServerCommand();
  std::vector<Device>* addedDevices;
};

#endif // COMMANDS_H
