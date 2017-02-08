#ifndef MESSAGES_H
#define MESSAGES_H
#include <QtCore>

using namespace std;

class Messages
{
public:
    Messages();
    QByteArray get_message(QString mWitch, QString deviceId);

private:
    quint8 qstringToQuint8(QString string);
    QByteArray ack_message;
    QByteArray crc_message;
    QByteArray success_message;
    QByteArray error_message;
    QByteArray stop_server;
    QByteArray restart_server;
    QByteArray reset_server;
};

#endif // MESSAGES_H