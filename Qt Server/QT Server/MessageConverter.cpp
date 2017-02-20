#include "MessageConverter.h"

MessageConverter::MessageConverter() {
}

std::string MessageConverter::qstringToString(QString text) {
  return text.toStdString();
}

QString MessageConverter::stringToQString(std::string text) {
  return QString::fromStdString(text);
}

std::vector<byte> MessageConverter::qbytearrayToCharArray(QByteArray byteArr) {
  std::vector<byte> charvector(byteArr.begin(), byteArr.end());
  return charvector;
}

QByteArray MessageConverter::bytesToQBytes(std::vector<byte> bytes) {
  QByteArray QbyteArr;
  for (auto i : bytes) {
    QbyteArr.push_back((quint8)(i));
  }
  return QbyteArr;
}

template<class T>
std::string MessageConverter::toString(const T& t) {
  std::ostringstream stream;
  stream << t;
  return stream.str();
}

std::string MessageConverter::byteToString(byte bytevalue) {
  return std::to_string((int)bytevalue);
}

std::string MessageConverter::qbytearrayToString(QByteArray byteArr) {
  std::string result = "";
  for (int i = 0; i < qbytearrayToCharArray(byteArr).size(); i++) {
    if (i < qbytearrayToCharArray(byteArr).size() - 1) {
      result += byteToString(qbytearrayToCharArray(byteArr)[i]) + " ";
    } else {
      result += byteToString(qbytearrayToCharArray(byteArr)[i]);
    }
  }
  return result;
}