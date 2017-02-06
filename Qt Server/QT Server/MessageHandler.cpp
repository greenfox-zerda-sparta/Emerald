#include "MessageHandler.h"

MessageHandler::MessageHandler() {
  validKeys = { "deviceID1", "deviceID2", "cmdID", "homeID", "floorID", "roomID", "groupID", "deviceIDLow", "deviceIDHigh" };
}
 
void MessageHandler::splitMessage(std::vector<unsigned char> bytes) {
  for (int i = 0; i < bytes.size(); i++) {
    if (i < validKeys.size()) {
      commandMap[validKeys[i]] = bytes[i];
    }
  }
}

std::unordered_map<std::string, unsigned char> MessageHandler::getCommandMap() {
  return commandMap;
}