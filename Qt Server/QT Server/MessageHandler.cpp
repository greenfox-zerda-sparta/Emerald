﻿#include "MessageHandler.h"

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

std::map<std::string, byte> MessageHandler::getmessageMap() {
  return messageMap;
}

void MessageHandler::makeCommand(std::vector<Device*>& addedDevices, QTcpSocket* client, std::vector<byte>& bytes,
  std::map<QTcpSocket*, Device*>& deviceMap, MessageConverter* msgConv) {
  // args will be used when message should be transferred
  Commands* cmd = new Commands();
  cmd->setMessageMap(messageMap);
  cmd->setAddedDevices(addedDevices);
  cmd->setDeviceMap(deviceMap);
  cmd->setBytes(bytes);
  cmd->runCommand();
  delete cmd;
}
