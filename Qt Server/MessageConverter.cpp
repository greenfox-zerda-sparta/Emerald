#include "MessageConverter.h"

MessageConverter::MessageConverter() {
}

std::string MessageConverter::qstringToString(QString text) {
  // keeps Unicode since QT 5.0, otherwise use text.toUtf8().constData();
  return text.toStdString();
}

QString MessageConverter::stringToQString(std::string text) {
  return QString::fromStdString(text);
}

template<class T>
std::string MessageConverter::toString(const T& t) {
  std::ostringstream stream;
  stream << t;
  return stream.str();
}

std::vector<unsigned char> MessageConverter::qbytearrayToCharArray(QByteArray byteArr) {
  std::vector<unsigned char> charvector(byteArr.begin(), byteArr.end());
  return charvector;
}