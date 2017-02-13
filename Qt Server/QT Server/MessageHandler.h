// #pragma once
#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include <vector>
#include <map>
#include <QTcpSocket>
#include "MessageConverter.h"

typedef unsigned char byte;

class MessageHandler {
private:
  std::vector<std::string> validKeys;
  std::map<std::string, byte> messageMap;
  std::vector<byte> msgToSend;
public:
  MessageHandler();
  void splitMessage(std::vector<byte> bytes);
  void executeCmd(QTcpSocket* client, std::vector<byte> bytes, std::map<QTcpSocket*, int>* ptr_socketmap, MessageConverter* msgConv);
  std::map<std::string, byte> getmessageMap();
};

#endif
