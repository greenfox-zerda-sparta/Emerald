#ifndef MESSAGECONVERTER_H
#define MESSAGECONVERTER_H

#include <sstream>
#include <string>
#include <vector>
#include <QtCore>

typedef unsigned char byte;

template<class T>
std::string ToString(const T& t) {
  std::ostringstream stream;
  stream << t;
  return stream.str();
}

class MessageConverter {
  public:
    MessageConverter();
    std::string QStringToString(QString text);
    QString StringToQString(std::string text);
    template<class T>
    std::string ToString(const T& t);
    std::vector<byte> QByteArrayToCharArray(QByteArray byteArr);
    QByteArray BytesToQBytes(std::vector<byte>);
    std::string ByteToString(byte b);
    std::string QByteArrayToString(QByteArray byteArr);
};

#endif
