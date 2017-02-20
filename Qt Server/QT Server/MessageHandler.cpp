#include "MessageHandler.h"

MessageHandler::MessageHandler() {
  keys = { "targetIDHigh", "targetIDLow", "cmdID", "homeID", "floorID", "roomID", "groupID",
           "senderIDHigh", "senderIDLow", "body1", "body2", "body3", "body4", "body5",
           "crc1", "crc2", "crc3"
         };
  validMsgLength = 17;
}

void MessageHandler::MakeMessageMap(std::vector<byte> bytes) {
  for (unsigned int i = 0; i < bytes.size(); i++) {
    if (i < validMsgLength) {
      messageMap[keys[i]] = bytes[i];
    }
  }
}

std::map<std::string, byte> MessageHandler::GetMessageMap() {
  return messageMap;
}

void MessageHandler::MakeCommand(std::vector<Device*>& addedDevices, std::vector<byte> bytes, std::map<QTcpSocket*, Device*>& deviceMap, MessageLogfile* msgLog) {
  MakeMessageMap(bytes);
  Commands* cmd = new Commands(addedDevices, msgLog);
  cmd->SetMessageMap(messageMap);
  cmd->SetDeviceMap(deviceMap);
  cmd->SetBytes(bytes);
  cmd->RunCommand();
  delete cmd;
}
