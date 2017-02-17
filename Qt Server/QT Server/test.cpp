#ifdef TEST
#define CATCH_CONFIG_MAIN
#include <QtCore>
#include <string>
#include <vector>
#include <map>
#include <string>
#include "catch.hpp"
#include "MessageConverter.h"
#include "SubDevice.h"
#include "UI.h"
#include "commands.h"
#include "MessageHandler.h"

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
  std::vector<unsigned char> uCharVector = { 'b', 'y', 't', 'e', ' ', 'a', 'r', 'r', 'a', 'y', '.' };
  REQUIRE(msgConv->qbytearrayToCharArray(qbyteArr) == uCharVector);
}

TEST_CASE("Checking UI constructor") {
  IDs ids;
  Device* ui = new UI(ids, "10.27.0.12");
  REQUIRE(ui->get_groupID() == 254);
  REQUIRE(ui->get_deviceIDHigh() == 255);
  REQUIRE_FALSE(ui->get_groupID() == 13);
  REQUIRE(ui->get_IP() == "10.27.0.12");
}

TEST_CASE("CommandMap function pointers call the right function from message: 253 - reset server ") {
  MessageHandler msgHandler;
  std::vector<byte> comm = { 255, 254, 253, 255, 255, 255, 254, 255, 253, 0, 0, 0, 0, 0, 0, 0, 0 };
  msgHandler.splitMessage(comm);
  std::map<std::string, unsigned char> mMap = msgHandler.getmessageMap();
  std::vector<Device*> Devices;
  Commands command(Devices);
  command.setMessageMap(mMap);
  command.runCommand();
  // should print to console: RESETTING SERVER
}

TEST_CASE("Add device command adds a new device with IP") {
  MessageHandler msgHandler;
  std::vector<byte> comm = { 255, 254, 247, 255, 2, 1, 1, 255, 253, 10, 27, 6, 21, 0, 0, 0, 0 };
  msgHandler.splitMessage(comm);
  std::map<std::string, unsigned char> mMap = msgHandler.getmessageMap();
  std::vector<Device*> Devices;
  Commands command(Devices);
  command.setMessageMap(mMap);
  command.runCommand();
  REQUIRE(Devices.size() == 1);
  REQUIRE(Devices[0]->get_groupID() == 1);
  REQUIRE(Devices[0]->get_IP() == "10.27.6.21");
}

#endif
