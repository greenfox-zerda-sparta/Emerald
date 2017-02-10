#include "MessageHandler.h"
#include "Logfile.h"
#include "MyTime.h"
#include <iostream>

MessageHandler::MessageHandler() {
  validKeys = { "targetIDHigh", "targetIDLow", "cmdID", "homeID", "floorID", "roomID", "groupID", "senderIDHigh", "senderIDLow" };
  // body and crc to be added
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

void MessageHandler::executeCmd(QTcpSocket* client, std::vector<byte> bytes, std::map<QTcpSocket*, int>* ptr_socketmap, MessageConverter* msgConv) {
  if (commandMap["targetIDHigh"] == 255 && commandMap["targetIDLow"] == 254 &&
    commandMap["senderIDHigh"] == 255 && commandMap["senderIDLow"] == 253) {
    switch (commandMap["cmdID"]) { // a case-ben indulhatna a thread. Vagy itt adja ki a parancsot, ami elinditja
    case 253:
      std::cout << "RESETTING SERVER\n" << std::endl; // reset server;  delete all devices
      break;
    case 254:
      std::cout << "RESTARTING SERVER\n" << std::endl; // restart server;
      break;
    case 255:
      std::cout << "STOPPING SERVER\n" << std::endl; //stop server;
      break;
    case 252:
      std::cout << "ACK\n" << std::endl; // ;
      break;
    case 242:
      std::cout << "UI status report: OK\n" << std::endl;
      break;
    case 240:
      std::cout << "UI status report: ERROR\n" << std::endl;
      break;
    case 241:
      std::cout << "Command reply: success.\n" << std::endl;
      break;
    case 251:
      std::cout << "UI status report: CRC ERROR\n" << std::endl;
      break;
    }
  } else {
    for (auto& otherClient : *ptr_socketmap) {               // for now: all other commands are sent to all devices
      if (otherClient.second != 0) {
        QTcpSocket* client = otherClient.first;
        client->write(msgConv->bytesToQBytes(bytes));
      }
    }
    std::string message = "Device " + toString((*ptr_socketmap)[client]) + ": ";
    for (auto iter : bytes) { message += toString(int(iter)) + ", "; }
    // mylogfile->log_buffer("Admin message " + LocalTimer->GetTimeFileFormat() + " " + message);
    std::cout << message << std::endl;
  }
}