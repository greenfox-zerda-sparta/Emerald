#include "MessageConverter.h"

MessageConverter::MessageConverter() {
}

std::string MessageConverter::QStringToString(QString text) {
  return text.toStdString();
}

QString MessageConverter::StringToQString(std::string text) {
  return QString::fromStdString(text);
}

std::vector<byte> MessageConverter::QByteArrayToCharArray(QByteArray byteArr) {
  std::vector<byte> charVector(byteArr.begin(), byteArr.end());
  return charVector;
}

QByteArray MessageConverter::BytesToQBytes(std::vector<byte> bytes) {
  QByteArray qByteArr;
  for (auto i : bytes) {
    qByteArr.push_back((quint8)(i));
  }
  return qByteArr;
}

template<class T>
std::string MessageConverter::ToString(const T& t) {
  std::ostringstream stream;
  stream << t;
  return stream.str();
}

std::string MessageConverter::ByteToString(byte byteValue) {
  return std::to_string((int)byteValue);
}