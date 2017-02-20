﻿// #pragma once
#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include <iostream>
#include <map>
#include <QTcpSocket>
#include <vector>
#include "commands.h"
#include "MessageLogfile.h"
#include "MyTime.h"

typedef unsigned char byte;

class MessageHandler {
  private:
    std::vector<std::string> Keys;
    std::map<std::string, byte> messageMap;
    std::vector<byte> msgToSend;
    int validMsgLength;
  public:
    MessageHandler();
    void MakeMessageMap(std::vector<byte> bytes);
    void MakeCommand(std::vector<Device*>& addedDevices, std::vector<byte> bytes, std::map<QTcpSocket*, Device*>& deviceMap);
    std::map<std::string, byte> GetMessageMap();
};

#endif
