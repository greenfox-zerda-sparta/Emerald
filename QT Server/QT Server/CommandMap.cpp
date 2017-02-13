#include "CommandMap.h"

CommandMap::CommandMap(Commands cmd) {
  ptr_resetServer = &Commands::resetServer;
  ptr_restartServer = &Commands::restartServer;
  ptr_stopServer = &Commands::stopServer;
  ptr_addDevice = &Commands::addDevice;
  ptr_removeDevice = &Commands::removeDevice;
  ptr_getStatusReport = &Commands::getStatusReport;
  ptr_setData = &Commands::setData;
  ptr_forwardMessage = &Commands::forwardMessage;

  cmdMap[239] = ptr_setData;
  cmdMap[246] = ptr_getStatusReport;
  cmdMap[247] = ptr_addDevice;
  cmdMap[248] = ptr_removeDevice;
  cmdMap[253] = ptr_resetServer;
  cmdMap[254] = ptr_restartServer;
  cmdMap[255] = ptr_stopServer;
  cmdMap[0] = ptr_forwardMessage;
  cmdMap[1] = ptr_forwardMessage;
  cmdMap[2] = ptr_forwardMessage;
  cmdMap[3] = ptr_forwardMessage;
  cmdMap[4] = ptr_forwardMessage;
  cmdMap[5] = ptr_forwardMessage;

  (cmd.*cmdMap[cmd.messageMap["cmdID"]])();
}