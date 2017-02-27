#include "MessageHandler.h"
#include "Server.h"

MessageHandler::MessageHandler(Server* server) {
  keys = { "targetIDHigh", "targetIDLow", "cmdID", "homeID", "floorID", "roomID", "groupID",
           "senderIDHigh", "senderIDLow", "body1", "body2", "body3", "body4", "body5",
           "crc1", "crc2", "crc3"
         };
  validMsgLength = 17;
  this->server = server;
}

void MessageHandler::MakeMessageMap(std::vector<byte> bytes) {
  for (unsigned int i = 0; i < bytes.size(); i++) {
    if (i < (unsigned int)validMsgLength) {
      messageMap[keys[i]] = bytes[i];
    }
  }
}

std::map<std::string, byte> MessageHandler::GetMessageMap() {
  return messageMap;
}

void MessageHandler::MakeCommand(std::vector<std::shared_ptr<Device>>& addedDevices, std::vector<byte> bytes, std::map<QTcpSocket*, std::shared_ptr<Device> >& deviceMap, std::shared_ptr<MessageLogfile> msgLog) {
  MakeMessageMap(bytes);
  Commands* cmd = new Commands(addedDevices, msgLog, server);
  cmd->SetMessageMap(messageMap);
  cmd->SetDeviceMap(deviceMap);
  cmd->SetBytes(bytes);
  cmd->RunCommand();
  delete cmd;
}
