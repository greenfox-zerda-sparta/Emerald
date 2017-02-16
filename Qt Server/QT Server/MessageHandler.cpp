#include "MessageHandler.h"

MessageHandler::MessageHandler() {
  validKeys = { "targetIDHigh", "targetIDLow", "cmdID", "homeID", "floorID", "roomID", "groupID", 
                "senderIDHigh", "senderIDLow", "body1", "body2", "body3", "body4", "body5", 
                "crc1", "crc2", "crc3" };
  // body and crc to be added
  validMsgLength = 17;
}

void MessageHandler::splitMessage(std::vector<byte> bytes) {
  for (unsigned int i = 0; i < validMsgLength; i++) {
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
