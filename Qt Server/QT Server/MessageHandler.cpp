#include "MessageHandler.h"
#include <iostream>

MessageHandler::MessageHandler() {
  validKeys = { "targetIDHigh", "targetIDLow", "senderIDHigh", "senderIDLow", "groupID", "homeID", "floorID", "roomID", "cmdID" };
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

void MessageHandler::executeCmd(std::vector<byte> bytes) {
  if (commandMap["targetIDHigh"] == 255 && commandMap["targetIDLow"] == 255) {
    switch (commandMap["cmdID"]) {
      case 253:
        std::cout << "RESETTING SERVER" ; // reset server;  delete all devices
      case 254:
        std::cout << "RESTARTING SERVER"; // restart server;
      case 255:
        std::cout << "STOPPING SERVER" << std::endl; //stop server;
    }
  }
}