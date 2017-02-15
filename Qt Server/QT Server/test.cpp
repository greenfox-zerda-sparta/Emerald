#ifdef TEST
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "MessageConverter.h"
#include "SubDevice.h"
#include "UI.h"
#include "commands.h"
#include "MessageHandler.h"
#include <QtCore>
#include <string>
#include <vector>
#include <map>
#include <string>

typedef unsigned char byte;

TEST_CASE("QString is converted to std string and back") {
  MessageConverter* msgConv;
  QString qstring = "This is a string.";
  std::string stdstring = "This is a string.";
  REQUIRE(msgConv->qstringToString(qstring) == stdstring);
  REQUIRE(msgConv->stringToQString(stdstring) == qstring);
}

TEST_CASE("QByteArray is convverted to unsigned char vector") {
  MessageConverter* msgConv;
  QByteArray qbyteArr = "byte array.";
  std::vector<unsigned char> uCharVector = {'b', 'y', 't', 'e', ' ', 'a', 'r', 'r', 'a', 'y', '.'};
  REQUIRE(msgConv->qbytearrayToCharArray(qbyteArr) == uCharVector);
}

TEST_CASE("Checking SubDevice device constructor") {
  //IDs ids;
  MessageHandler msgHandler;
  std::vector<byte> comm = { 255, 254, 247, 255, 2, 1, 1, 255, 253 };
  msgHandler.splitMessage(comm);
  std::map<std::string, unsigned char> mMap = msgHandler.getmessageMap();
  Device* device = new SubDevice(mMap);
  REQUIRE(device->get_groupID() == 1);
 // REQUIRE(device->get_IP() == "xxx");
}

//TEST_CASE("Checking UI constructor") {
//  IDs ids;
//  Device* ui = new UI(ids, "xxx");
//  REQUIRE(ui->get_groupID() == 254);
//  REQUIRE(ui->get_deviceIDHigh() == 255);
//  REQUIRE_FALSE(ui->get_groupID() == 13);
//  REQUIRE(ui->get_IP() == "xxx");
//}

TEST_CASE("CommandMap function pointers call the right function from message: 253 - reset server ") {
  MessageHandler msgHandler;
  std::vector<byte> comm = { 255, 254, 253, 255, 255, 255, 254, 255, 253 };
  msgHandler.splitMessage(comm);
  std::map<std::string, unsigned char> mMap = msgHandler.getmessageMap();
  Commands command();
  command.setMessageMap(mMap);
  command.runCommand();
  // should print to console: RESETTING SERVER
}

#endif
