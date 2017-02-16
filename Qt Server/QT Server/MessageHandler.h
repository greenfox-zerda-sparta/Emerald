// #pragma once
#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include <vector>
#include <map>
#include <QTcpSocket>
#include <iostream>
#include "Logfile.h"
#include "MyTime.h"
#include "MessageConverter.h"
#include "commands.h"

typedef unsigned char byte;

class MessageHandler {
private:
  std::vector<std::string> validKeys;
  std::map<std::string, byte> messageMap;
  std::vector<byte> msgToSend;
  int validMsgLength;
public:
  MessageHandler();
  void splitMessage(std::vector<byte> bytes);
  void makeCommand(std::vector<Device*>& addedDevices, QTcpSocket* client, std::vector<byte>& bytes,
    std::map<QTcpSocket*, Device*>& deviceMap, MessageConverter* msgConv);
  std::map<std::string, byte> getmessageMap();
};

#endif
