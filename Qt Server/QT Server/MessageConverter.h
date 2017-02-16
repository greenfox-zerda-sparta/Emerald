#ifndef MESSAGECONVERTER_H
#define MESSAGECONVERTER_H

#include <QtCore>
#include <string>
#include <vector>
#include <sstream>

typedef unsigned char byte;

class MessageConverter {
public:
  MessageConverter();
  std::string qstringToString(QString text);
  QString stringToQString(std::string text);
  template<class T>
  std::string toString(const T& t);
  std::vector<byte> qbytearrayToCharArray(QByteArray byteArr);
  QByteArray bytesToQBytes(std::vector<byte>);
  std::string byteToString(byte b);
};

#endif
