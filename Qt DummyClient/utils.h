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

class Utils {
  public:
    static quint8 qstringToQuint8(QString string) {
      QTextStream ts(&string);
      quint16 result1;
      ts >> result1;
      quint8 result = (quint8) result1;
//        qDebug() << result;
      return result;
    }

    static QString messageToNumbers(QByteArray message) {
      QString ret = "";
      for(int i = 0; i < message.length(); ++i) {
        ret += QString::number((int) message[i]);
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
};

#endif // UTILS_H
