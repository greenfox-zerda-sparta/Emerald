#ifndef MESSAGES_H
#define MESSAGES_H
#include <QtCore>
#include "utils.h"

using namespace std;

class Messages
{
public:
    Messages();
    QByteArray get_message(QString mWitch, Dev deviceId);

private:
//    quint8 qstringToQuint8(QString string);
    QByteArray ack_message;
    QByteArray crc_message;
    QByteArray success_message;
    QByteArray error_message;
    QByteArray stop_server;
    QByteArray restart_server;
    QByteArray reset_server;
    QByteArray add_device;
};

#endif // MESSAGES_H
