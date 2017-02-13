#ifndef MESSAGES_H
#define MESSAGES_H
#include <QtCore>
#include <vector>

typedef unsigned char byte;

class Messages
{
public:
    Messages();
    QByteArray getMessage(byte& messageId);
private:
    QByteArray message;
    bool isValidMessageID(byte& messageId);
};

#endif // MESSAGES_H
