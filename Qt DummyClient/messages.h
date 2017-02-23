#ifndef MESSAGES_H
#define MESSAGES_H
#include <QtCore>
#include "utils.h"
#include <iostream>

using namespace std;

class Messages {
  public:
    Messages();
    QByteArray get_message(QString mWitch, Dev& device);
    QByteArray getNextMessage(QString message, Dev& device);
    QByteArray getAddDeviceMessage(Dev& device, QString newDevDescription);
    QByteArray getRemoveDeviceMessage(Dev& device, QString id);
    void setDevice(QString message, Dev& device, int setStatus = 0);
  private:
    QByteArray error_message;
};

#endif // MESSAGES_H
