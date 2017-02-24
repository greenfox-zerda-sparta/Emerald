#ifndef MESSAGES_H
#define MESSAGES_H

#include <iostream>
#include <vector>
#include <QtCore>

typedef unsigned char byte;

class Messages {
  public:
    Messages();
    std::vector<byte> GetMessage(byte messageId, byte body1 = 0, byte body2 = 0, byte body3 = 0, byte body4 = 0, byte body5 = 0);
  private:
    std::vector<byte> message;
    bool IsValidMessageID(byte messageId);
};

#endif // MESSAGES_H
