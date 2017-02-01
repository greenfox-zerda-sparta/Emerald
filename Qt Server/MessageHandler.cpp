#include "MessageHandler.h"

MessageHandler::MessageHandler() {
}

MessageHandler::~MessageHandler() {
}

void MessageHandler::splitMessage(QByteArray bytes) {
  targetID = bytes[0];
  cmdID = bytes[1];
  homeID = bytes[2];
  floorID = bytes[3];
  roomID = bytes[4];
  groupID = bytes[5];
  deviceIDLow = bytes[6];
  deviceIDHigh = bytes[7];
}

void MessageHandler::toFullCommand(QByteArray bytes) {
  splitMessage(bytes);
  fullCommand.clear();
  for (int i = 0; i < bytes.size(); i++) {
    fullCommand.push_back(bytes[i]);
  }
}

std::vector<char> MessageHandler::getFullCommand() {
  return fullCommand;
}

//admin client has to send at least 8 characters in a message!!!