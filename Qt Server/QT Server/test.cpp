#ifdef TEST
#define CATCH_CONFIG_MAIN

#include <map>
#include <string>
#include <vector>
#include <QtCore>
#include "catch.hpp"
#include "commands.h"
#include "DeviceLogfile.h"
#include "MessageConverter.h"
#include "MessageHandler.h"
#include "SubDevice.h"
#include "UI.h"

typedef unsigned char byte;

TEST_CASE("QString is converted to std string and back") {
  MessageConverter* msgConv;
  QString qstring = "This is a string.";
  std::string stdstring = "This is a string.";
  REQUIRE(msgConv->QStringToString(qstring) == stdstring);
  REQUIRE(msgConv->StringToQString(stdstring) == qstring);
}

TEST_CASE("QByteArray is convverted to unsigned char vector") {
  MessageConverter* msgConv;
  QByteArray qbyteArr = "byte array.";
  std::vector<unsigned char> uCharVector = { 'b', 'y', 't', 'e', ' ', 'a', 'r', 'r', 'a', 'y', '.' };
  REQUIRE(msgConv->QByteArrayToCharArray(qbyteArr) == uCharVector);
}

TEST_CASE("Checking UI constructor") {
  IDs ids{255, 253, 254, 255, 255, 255};
  std::string ip = "10.27.0.12";
  Device* ui = new UI(ids, ip, 1);
  REQUIRE(ui->GetGroupID() == 254);
  REQUIRE(ui->GetDeviceIDHigh() == 255);
  REQUIRE_FALSE(ui->GetGroupID() == 13);
  REQUIRE(ui->GetIP() == "10.27.0.12");
}

TEST_CASE("CommandMap function pointers call the right function from message: 253 - reset server ") {
  MessageHandler msgHandler;
  std::vector<byte> comm = { 255, 254, 253, 255, 255, 255, 254, 255, 253, 0, 0, 0, 0, 0, 0, 0, 0 };
  msgHandler.MakeMessageMap(comm);
  std::map<std::string, unsigned char> mMap = msgHandler.GetMessageMap();
  std::vector<Device*> Devices;
  Commands command(Devices);
  command.SetMessageMap(mMap);
  command.RunCommand();
  // should print to console: RESETTING SERVER
}

TEST_CASE("Add device command adds a new device with IP") {
  MessageHandler msgHandler;
  std::vector<byte> comm = { 255, 254, 247, 255, 2, 1, 1, 255, 253, 10, 27, 6, 21, 0, 0, 0, 0 };
  msgHandler.MakeMessageMap(comm);
  std::map<std::string, unsigned char> mMap = msgHandler.GetMessageMap();
  std::vector<Device*> Devices;
  Commands command(Devices);
  command.SetMessageMap(mMap);
  command.RunCommand();
  REQUIRE(Devices.size() == 1);
  REQUIRE(Devices[0]->GetGroupID() == 1);
  REQUIRE(Devices[0]->GetIP() == "10.27.6.21");
}

TEST_CASE("DeviceLogfile class getDevice(...) method can build a device from serialized one") {
  DeviceLogfile logf;
  UI ui = UI({ 255, 253, 254, 255, 255, 255}, "127.0.0.1", 1);
  std::string buffer = "255 253 254 255 255 255 127.0.0.1 2";
  Device* gotDev = logf.getDevice(buffer);
  REQUIRE(ui.GetDeviceIDHigh() == gotDev->GetDeviceIDHigh());
  REQUIRE(ui.GetDeviceIDLow() == gotDev->GetDeviceIDLow());
  REQUIRE(ui.GetGroupID() == gotDev->GetGroupID());
  REQUIRE(ui.GetHomeID() == gotDev->GetHomeID());
  REQUIRE(ui.GetFloorID() == gotDev->GetFloorID());
  REQUIRE(ui.GetRoomID() == gotDev->GetRoomID());
  REQUIRE(ui.GetIP() == gotDev->GetIP());
  REQUIRE_FALSE(ui.IsWorking() == gotDev->IsWorking());
}


#endif
