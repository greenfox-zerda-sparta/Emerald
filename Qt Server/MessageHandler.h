// #pragma once
#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include <vector>
#include <unordered_map>

class MessageHandler {
private:
  std::vector<std::string> validKeys;
  std::unordered_map<std::string, unsigned char> commandMap;
public:
  MessageHandler();
  void splitMessage(std::vector<unsigned char> bytes);
  std::unordered_map<std::string, unsigned char> getCommandMap();
};

#endif
