#ifndef MESSAGES_H
#define MESSAGES_H
#include <QtCore>
#include "utils.h"
#include<vector>
using namespace std;
typedef unsigned char uchar;

class Messages
{
public:
    Messages();
    QByteArray get_message(QString mWitch, Dev deviceId);
    uchar* get_m(std::string mWitch);

private:
//    quint8 qstringToQuint8(QString string);
    QByteArray ack_message;
    QByteArray crc_message;
    QByteArray success_message;
    QByteArray error_message;
    QByteArray stop_server;
    QByteArray restart_server;
    QByteArray reset_server;
    vector<uchar> ack;
};

#endif // MESSAGES_H
