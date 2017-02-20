#ifndef MESSAGES_H
#define MESSAGES_H
#include <QtCore>
#include <vector>

typedef unsigned char byte;

class Messages {
  public:
    Messages();
    QByteArray getMessage(byte messageId, byte body1 = 0, byte body2 = 0, byte body3 = 0, byte body4 = 0, byte body5 = 0);
  private:
    QByteArray message;
    bool isValidMessageID(byte messageId);
};

#endif // MESSAGES_H
