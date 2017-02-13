#ifndef COMMANDMAP_H
#define COMMANDMAP_H
#include <map>
#include <vector>

#include "commands.h"

typedef unsigned char byte;

class CommandMap {
private:
  void(Commands::*ptr_resetServer)();
  void(Commands::*ptr_restartServer)();
  void(Commands::*ptr_stopServer)();
  void(Commands::*ptr_addDevice)();
  void(Commands::*ptr_removeDevice)();
  void(Commands::*ptr_getStatusReport)();
  void(Commands::*ptr_setData)();
  void(Commands::*ptr_forwardMessage)();
public:
  std::map<byte, void(Commands::*)()> cmdMap;
  CommandMap(Commands cmd);
};

#endif
