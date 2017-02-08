#ifdef TEST
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "MessageConverter.h"

#include <QtCore>
#include <string>
#include <vector>

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
#endif