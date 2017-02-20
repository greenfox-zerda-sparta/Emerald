#include "messages.h"

using namespace std;

Messages::Messages() {
  message = vector<byte> {255, 253, 240, 255, 255, 255, 254, 255, 254, 0, 0, 0, 0, 0, 0, 0, 0};
}

bool Messages::isValidMessageID(byte messageId) {
  return (messageId == 240 || //cmd rply - ERROR
          messageId == 241 || //cmd rply - SUCCESS
          messageId == 242 || //STATE REPORT
          messageId == 249 || // DEVICE DISCONNECTED
          messageId == 250 || // NO ANSWER - DEVICE ERROR
          messageId == 251 || // CRC ERROR <<<<<<<<< This could be sent to devices too >>>>>>>>>
          messageId == 252);  // ACK
}

std::vector<byte> Messages::getMessage(byte messageId, byte body1, byte body2, byte body3, byte body4, byte body5) {
  vector<byte> retMessage = message;
  if(isValidMessageID(messageId)) {
    retMessage[2] = messageId;
    retMessage[9] = body1;
    retMessage[10] = body2;
    retMessage[11] = body3;
    retMessage[12] = body4;
    retMessage[13] = body5;
  }
  return retMessage;
}
