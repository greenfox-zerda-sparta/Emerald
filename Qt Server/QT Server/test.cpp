#ifdef TEST
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "MessageConverter.h"
#include "Lamp.h"
#include "UI.h"
#include "commands.h"
#include "CommandMap.h"
#include "MessageHandler.h"

#include <QtCore>
#include <string>
#include <vector>
#include <map>
#include <string>

typedef unsigned char byte;

TEST_CASE("QString is converted to std string and back")
{
  MessageConverter* msgConv;
  QString qstring = "This is a string.";
  std::string stdstring = "This is a string.";
  REQUIRE(msgConv->qstringToString(qstring) == stdstring);
  REQUIRE(msgConv->stringToQString(stdstring) == qstring);
}

TEST_CASE("QByteArray is convverted to unsigned char vector")
{
  MessageConverter* msgConv;
  QByteArray qbyteArr = "byte array.";
  std::vector<unsigned char> uCharVector = {'b', 'y', 't', 'e', ' ', 'a', 'r', 'r', 'a', 'y', '.'};
  REQUIRE(msgConv->qbytearrayToCharArray(qbyteArr) == uCharVector);
}

TEST_CASE("Checking Lamp device constructor")
{
  IDs ids;

  Device* lamp = new Lamp(ids, "xxx");

  REQUIRE(lamp->get_groupID() == 1);
  //REQUIRE(lamp->get_deviceIDHigh() == 0);
  REQUIRE(lamp->get_IP() == "xxx");
}

TEST_CASE("Checking UI constructor") {
  IDs ids;

  Device* ui = new UI(ids, "xxx");

  REQUIRE(ui->get_groupID() == 254);
  REQUIRE(ui->get_deviceIDHigh() == 255);
  REQUIRE_FALSE(ui->get_groupID() == 13);
  REQUIRE(ui->get_IP() == "xxx");
}

TEST_CASE("CommandMap function pointers call the right function: 253 - reset server ") {
  MessageHandler msgHandler;
  std::vector<byte> comm = { 255, 253, 253, 255, 255, 255, 254, 255, 254 };
  msgHandler.splitMessage(comm);
  Commands command(msgHandler.getmessageMap());

  REQUIRE(command.messageMap["cmdID"] == 253);
  CommandMap commMap();
  //commMap.*cmdMap[253](); 
}

#endif
