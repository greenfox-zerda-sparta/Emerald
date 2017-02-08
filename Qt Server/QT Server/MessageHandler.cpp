#include "MessageHandler.h"

MessageHandler::MessageHandler() {
  validKeys = { "deviceIDHigh", "deviceIDLow", "groupID", "homeID", "floorID", "roomID", "cmdID" };
}

void MessageHandler::splitMessage(std::vector<byte> bytes) {
  for (int i = 0; i < bytes.size(); i++) {
    if (i < validKeys.size()) {
      commandMap[validKeys[i]] = bytes[i];
    }
  }
}

std::unordered_map<std::string, byte> MessageHandler::getCommandMap() {
  return commandMap;
}