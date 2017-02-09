#ifndef UTILS_H
#define UTILS_H
#include <QtCore>

struct Dev {
    quint8 deviceIdHigh;
    quint8 deviceIdLow;
    quint8 homeId;
    quint8 floorId;
    quint8 roomId;
    quint8 groupId;
};

class Utils {
public:
    static quint8 qstringToQuint8(QString string){
        QTextStream ts(&string);
        quint16 result1;
        ts >> result1;
        quint8 result = (quint8) result1;
        qDebug() << result;
        return result;
    }
};

#endif // UTILS_H
