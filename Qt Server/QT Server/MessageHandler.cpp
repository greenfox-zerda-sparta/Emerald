#include "MessageHandler.h"
#include "Logfile.h"
#include "MyTime.h"
#include <iostream>

MessageHandler::MessageHandler() {
  validKeys = { "targetIDHigh", "targetIDLow", "cmdID", "homeID", "floorID", "roomID", "groupID", "senderIDHigh", "senderIDLow" };
  // body and crc to be added
}

void MessageHandler::splitMessage(std::vector<byte> bytes) {
  for (unsigned int i = 0; i < bytes.size(); i++) {
    if (i < validKeys.size()) {
      messageMap[validKeys[i]] = bytes[i];
    }
  }
}

void MessageHandler::executeCmd(QTcpSocket* client, std::vector<byte> bytes, std::map<QTcpSocket*, int>* ptr_socketmap, MessageConverter* msgConv) {
}
