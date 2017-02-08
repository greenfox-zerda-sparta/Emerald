// #pragma once
#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include <vector>
#include <unordered_map>

typedef unsigned char byte;

class MessageHandler {
private:
  std::vector<std::string> validKeys;
  std::unordered_map<std::string, byte> commandMap;
public:
  MessageHandler();
  void splitMessage(std::vector<byte> bytes);
  std::unordered_map<std::string, byte> getCommandMap();
  void MessageHandler::executeCmd(std::vector<byte> bytes);
};

#endif
