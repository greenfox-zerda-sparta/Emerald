#include "messages.h"

using namespace std;

Messages::Messages() {
  vector<byte> ack = vector<byte> {255, 253, 240, 255, 255, 255, 254, 255, 254};
  for(auto i: ack) {
    message.push_back((quint8)(i));
  }
}

bool Messages::isValidMessageID(byte& messageId) {
  return (messageId == 240 || //cmd rply - ERROR
          messageId == 241 || //cmd rply - SUCCESS
          messageId == 242 || //STATE REPORT
          messageId == 249 || // DEVICE DISCONNECTED
          messageId == 250 || // NO ANSWER - DEVICE ERROR
          messageId == 251 || // CRC ERROR <<<<<<<<< This could be sent to devices too >>>>>>>>>
          messageId == 252);  // ACK
}

QByteArray Messages::getMessage(byte& messageId) {
  QByteArray retMessage = message;
  if(isValidMessageID(messageId)) {
    retMessage[2] = messageId;
  }
  return retMessage;
}
