// #pragma once
#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include <vector>
#include <unordered_map>
#include <map>
#include <QTcpSocket>
#include "MessageConverter.h"

typedef unsigned char byte;

class MessageHandler {
private:
  std::vector<std::string> validKeys;
  std::unordered_map<std::string, byte> commandMap;
  std::vector<byte> msgToSend;
public:
  MessageHandler();
  void splitMessage(std::vector<byte> bytes);
  std::unordered_map<std::string, byte> getCommandMap();
  void executeCmd(QTcpSocket* client, std::vector<byte> bytes, std::map<QTcpSocket*, int>* ptr_socketmap, MessageConverter* msgConv);
};

#endif
