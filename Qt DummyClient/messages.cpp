#include "messages.h"

Messages::Messages()
{
    QByteArray msg;
    msg.append(qstringToQuint8("255"));
    msg.append(qstringToQuint8("254"));
    msg.append(qstringToQuint8("254"));
    msg.append(qstringToQuint8("0"));
    msg.append(qstringToQuint8("0"));
    msg.append(qstringToQuint8("0"));
    msg.append(qstringToQuint8("0"));
    msg.append(qstringToQuint8("255"));
    msg.append(qstringToQuint8("253"));
    restart_server = msg;
    msg.at(2) = qstringToQuint8("253");
    reset_server = msg;
    msg.at(2) = qstringToQuint8("255");
    stop_server = msg;
    msg.at(2) = qstringToQuint8("252");
    ack_message = msg;
    msg.at(2) = qstringToQuint8("251");
    crc_message = msg;
    msg.at(2) = qstringToQuint8("241");
    success_message = msg;
    msg.at(2) = qstringToQuint8("240");
    error_message = msg;
}

QByteArray Messages::get_message(QString mWitch, QString deviceId) {

    quint8 myIdHigh = qstringToQuint8(deviceId);
    quint8 myIdLow = qstringToQuint8(deviceId);
    return reset_server;
}

quint8 Messages::qstringToQuint8(QString string){
    QTextStream ts(&string);
    quint16 result1;
    ts >> result1;
    quint8 result = (quint8) result1;
    qDebug() << result;
    return result;
}
