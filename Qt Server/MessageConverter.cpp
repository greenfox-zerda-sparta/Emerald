#include "MessageConverter.h"

MessageConverter::MessageConverter() {
}

std::string MessageConverter::qstringToString(QString text) {
  return text.toStdString();
}

QString MessageConverter::stringToQString(std::string text) {
  return QString::fromStdString(text);
}

std::vector<unsigned char> MessageConverter::qbytearrayToCharArray(QByteArray byteArr) {
  std::vector<unsigned char> charvector(byteArr.begin(), byteArr.end());
  return charvector;
}

template<class T>
std::string MessageConverter::toString(const T& t) {
  std::ostringstream stream;
  stream << t;
  return stream.str();
}