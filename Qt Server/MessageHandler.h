// #pragma once
#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include <vector>
#include <QtCore>

class MessageHandler {
private:
  char targetID;
  char cmdID;
  char homeID;
  char floorID;
  char roomID;
  char groupID;
  char deviceIDLow;
  char deviceIDHigh;
  std::vector<char> fullCommand;
public:
  MessageHandler();
  ~MessageHandler();
  void splitMessage(QByteArray bytes);
  void toFullCommand(QByteArray bytes);
  std::vector<char> getFullCommand();
};

#endif
