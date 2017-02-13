#ifndef COMMANDMAP_H
#define COMMANDMAP_H
#include <map>
#include <vector>

#include "commands.h"

typedef unsigned char byte;

class CommandMap {
private:
  void(Commands::*ptr_resetServer)();
public:
  std::map<byte, void(Commands::*)()> cmdMap;
  CommandMap(Commands cmd);
};

#endif
