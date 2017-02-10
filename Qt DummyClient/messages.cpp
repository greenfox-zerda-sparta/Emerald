#include "messages.h"

Messages::Messages()
{
    QByteArray msg;
    msg.append(Utils::qstringToQuint8("255")); //target id high
    msg.append(Utils::qstringToQuint8("254")); //target id low
    msg.append(Utils::qstringToQuint8("254")); //command code
    msg.append(Utils::qstringToQuint8("255")); //home id
    msg.append(Utils::qstringToQuint8("255")); //floor id
    msg.append(Utils::qstringToQuint8("255")); //room id
    msg.append(Utils::qstringToQuint8("255")); //goup id
    msg.append(Utils::qstringToQuint8("255")); //device id high
    msg.append(Utils::qstringToQuint8("253")); //device id low
    restart_server = msg;
    msg[2] = Utils::qstringToQuint8("253");
    reset_server = msg;
    msg[2] = Utils::qstringToQuint8("255");
    stop_server = msg;
    msg[2] = Utils::qstringToQuint8("252");
    ack_message = msg;
    msg[2] = Utils::qstringToQuint8("251");
    crc_message = msg;
    msg[2] = Utils::qstringToQuint8("241");
    success_message = msg;
    msg[2] = Utils::qstringToQuint8("240");
    error_message = msg;
}

QByteArray Messages::get_message(QString mWitch, Dev device) {
    QByteArray msg;
    if (mWitch == "1")msg = stop_server;
    if (mWitch == "2")msg = restart_server;
    if (mWitch == "3")msg = reset_server;
    if (mWitch == "ack")msg = ack_message;
    if (mWitch == "crc")msg = crc_message;
    if (mWitch == "suc")msg = success_message;
    if (mWitch == "err")msg = error_message;
    else msg = ack_message;
    msg[7] = device.deviceIdHigh;
    msg[8] = device.deviceIdLow;
    return msg;
}
