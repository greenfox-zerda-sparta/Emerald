#include "CommandMap.h"

CommandMap::CommandMap() {
  ptr_resetServer = &Commands::resetServer;
  cmdMap[253] = ptr_resetServer;
  (this->*cmdMap[253])();
  /* void(*ptr_restartServer)() = cmd.restartServer;
  void(*ptr_stopServer)() = cmd.stopServer;
  void(*ptr_addDevice)() = cmd.addDevice;
  void(*ptr_removeDevice)() = cmd.removeDevice;
  void(*ptr_getStatusReport)() = cmd.getStatusReport;
  void(*ptr_setData)() = cmd.setData;
  void(*ptr_forwardMessage)() = cmd.forwardMessage;*/

  //cmdMap[239] = ptr_setData;
  //cmdMap[246] = ptr_getStatusReport;
  //cmdMap[247] = ptr_addDevice;
  //cmdMap[248] = ptr_removeDevice;

  /* cmdMap[254] = ptr_restartServer;
  cmdMap[255] = ptr_stopServer;
  cmdMap[0] = ptr_forwardMessage;
  cmdMap[1] = ptr_forwardMessage;
  cmdMap[2] = ptr_forwardMessage;
  cmdMap[3] = ptr_forwardMessage;
  cmdMap[4] = ptr_forwardMessage;
  cmdMap[5] = ptr_forwardMessage;*/
}