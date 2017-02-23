#ifndef UTILS_H
#define UTILS_H
#include <QtCore>
#include <vector>

typedef unsigned char byte;

struct Dev {
  quint8 deviceIdHigh;
  quint8 deviceIdLow;
  quint8 homeId;
  quint8 floorId;
  quint8 roomId;
  quint8 groupId;
  quint8 status;
};

union Ptr {
        unsigned char* asByte;
        uint32_t* asUInt;
};

class Utils {
  public:
    static quint8 qstringToQuint8(QString string) {
      QTextStream ts(&string);
      quint16 result1;
      ts >> result1;
      quint8 result = (quint8) result1;
      return result;
    }

    static QString messageToNumbers(QByteArray message) {
      QString ret = "";
      for(int i = 0; i < message.length(); ++i) {
        ret += QString::number((quint8)message[i]);
        ret += " ";
      }
      return ret.trimmed();
    }

    static QByteArray qstringnumbersToQByteArray(QString message) {
      QByteArray ret;
      QStringList list = message.split(' ');
      for(int i = 0; i < list.length(); ++i) {
        ret.append(qstringToQuint8(list[i]));
      }
      return ret;
    }

    static QByteArray qint32ToQByteArray(quint32 number) {
      QByteArray ret;
      unsigned char buffer[4];
      Ptr memoryArea;
      memoryArea.asByte = buffer;
      *memoryArea.asUInt = number;
      memoryArea.asByte = buffer;
      ret[0] = *memoryArea.asByte++;
      ret[1] = *memoryArea.asByte++;
      ret[2] = *memoryArea.asByte++;
      ret[3] = *memoryArea.asByte;
      return ret;
    }
};

#endif // UTILS_H
