#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <QTcpSocket>
#include "Commands.h"
#include "MessageLogfile.h"
#include "MyTime.h"

typedef unsigned char byte;

class Server;
class MessageHandler {
  private:
    std::vector<std::string> keys;
    std::map<std::string, byte> messageMap;
    std::vector<byte> msgToSend;
    int validMsgLength;
    Server* server;
  public:
    MessageHandler(Server* server = nullptr);
    void MakeMessageMap(std::vector<byte> bytes);
    void MakeCommand(std::vector<std::shared_ptr<Device>>& addedDevices, std::vector<byte> bytes, std::map<QTcpSocket*, std::shared_ptr<Device>>& deviceMap, std::shared_ptr<MessageLogfile> msgLog);
    std::map<std::string, byte> GetMessageMap();
};

#endif
