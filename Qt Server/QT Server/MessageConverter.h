#ifndef MESSAGECONVERTER_H
#define MESSAGECONVERTER_H

#include <QtCore>
#include <string>
#include <vector>

typedef unsigned char byte;

class MessageConverter {
public:
  MessageConverter();
  std::string qstringToString(QString text);
  QString stringToQString(std::string text);
  template<class T>
  std::string toString(const T& t);
  std::vector<byte> qbytearrayToCharArray(QByteArray byteArr);
};

#endif
