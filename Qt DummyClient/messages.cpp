#include "messages.h"

Messages::Messages() {
  QByteArray msg;
  msg.append(Utils::qstringToQuint8("255")); //target id high
  msg.append(Utils::qstringToQuint8("254")); //target id low /server
  msg.append(Utils::qstringToQuint8("240")); //command code :: 240 - error message
  msg.append(Utils::qstringToQuint8("255")); //home id
  msg.append(Utils::qstringToQuint8("255")); //floor id
  msg.append(Utils::qstringToQuint8("255")); //room id
  msg.append(Utils::qstringToQuint8("255")); //goup id
  msg.append(Utils::qstringToQuint8("255")); //device id high
  msg.append(Utils::qstringToQuint8("253")); //device id low /ui
  msg.append(Utils::qstringToQuint8("0"));   //body 1
  msg.append(Utils::qstringToQuint8("0"));   //body 2
  msg.append(Utils::qstringToQuint8("0"));   //body 3
  msg.append(Utils::qstringToQuint8("0"));   //body 4
  msg.append(Utils::qstringToQuint8("0"));   //body 5
  msg.append(Utils::qstringToQuint8("0"));   //crc 1
  msg.append(Utils::qstringToQuint8("0"));   //crc 2
  msg.append(Utils::qstringToQuint8("0"));   //crc 3
  error_message = msg;
}

QByteArray Messages::get_message(QString mWitch, Dev& device) {
  QByteArray msg = error_message; // Cmd - Error :: NTD
  if (mWitch == "1" || mWitch == "255") {
    msg[2] = Utils::qstringToQuint8("255"); // Stop server
  }
  if (mWitch == "2" || mWitch == "254") {
    msg[2] = Utils::qstringToQuint8("254"); // Restart server
  }
  if (mWitch == "3" || mWitch == "253") {
    msg[2] = Utils::qstringToQuint8("253"); // Reset server
  }
  if (mWitch == "add" || mWitch == "247") {
    msg[2] = Utils::qstringToQuint8("247");// Cmd - Add device
  }
  if (mWitch == "rem" || mWitch == "248") {
    msg[2] = Utils::qstringToQuint8("248");// Cmd - Remove device
  }
  if (mWitch == "ack" || mWitch == "252") {
    if(device.groupId != 253) {
      msg[1] = Utils::qstringToQuint8("253"); // UI
    }
    msg[2] = Utils::qstringToQuint8("252");// ACK
  }
  if (mWitch == "crc" || mWitch == "251") {
    if(device.groupId != 253) {
      msg[1] = Utils::qstringToQuint8("253"); // UI
    }
    msg[2] = Utils::qstringToQuint8("251");// Cmd - CRC error
  }
  if (mWitch == "suc" || mWitch == "241") {
    if(device.groupId != 253) {
      msg[1] = Utils::qstringToQuint8("253"); // UI
    }
    msg[2] = Utils::qstringToQuint8("241"); // Cmd - Success
  }
  if (mWitch == "sta" || mWitch == "242") {
    if(device.groupId != 253) {
      msg[1] = Utils::qstringToQuint8("253"); // UI
    }
    msg[2] = Utils::qstringToQuint8("242"); // Status report
  }
  msg[4] = device.floorId;
  msg[5] = device.roomId;
  msg[6] = device.groupId;
  msg[7] = device.deviceIdHigh;
  msg[8] = device.deviceIdLow;
  msg[9] = device.status; //body 1
  if (mWitch == "cst" || mWitch == "246") {
    msg[0] = Utils::qstringToQuint8("0");// all
    msg[1] = Utils::qstringToQuint8("0");// all
    msg[2] = Utils::qstringToQuint8("246"); // Command - get status report
    msg[4] = Utils::qstringToQuint8("0");// all;
    msg[5] = Utils::qstringToQuint8("0");// all;
    msg[6] = Utils::qstringToQuint8("0");// all;
    msg[9] = Utils::qstringToQuint8("0");// no body;
  }
  return msg;
}

void Messages::setDevice(QString message, Dev& device, int setStatus) {
  QByteArray messArray = Utils::qstringnumbersToQByteArray(message);
  device.deviceIdHigh = messArray[0];
  device.deviceIdLow = messArray[1];
  device.homeId = messArray[3];
  device.floorId = messArray[4];
  device.roomId = messArray[5];
  device.groupId = messArray[6];
  if(setStatus == 1) { //Set to value
    device.status = messArray[9];
  } else if(setStatus == 2) {  //Add value
    int newStatus = (int)device.status + (int)messArray[9];
    device.status = (newStatus > 100?(quint8)100:(quint8)newStatus);
  } else if(setStatus == 3) {  //Distract value
    int newStatus = (int)device.status - (int)messArray[9];
    device.status = (newStatus < 0?(quint8)0:(quint8)newStatus);
  }
}

QByteArray Messages::getNextMessage(QString message, Dev& device) {
  QByteArray nextMessArray;
  QStringList list = message.split(' ');
  QString commandId = list[2];
  if(commandId == "246") { //get status report
    setDevice(message, device);
    nextMessArray = get_message("242", device); //status report message
  } else if (commandId == "0" || commandId == "1" || commandId == "2") { //on-off-standby
    setDevice(message, device);
    nextMessArray = get_message("241", device); // Cmd - Success
  } else if (commandId == "3") { //Set(toValue)
    setDevice(message, device, 1);
    nextMessArray = get_message("242", device); // Status report
  } else if (commandId == "4") { //Set+(Value)
    setDevice(message, device, 2);
    nextMessArray = get_message("242", device); // Status report
  } else if (commandId == "5") { //Set-(Value)
    setDevice(message, device, 3);
    nextMessArray = get_message("242", device); // Status report
  }
  return nextMessArray;
}

QByteArray Messages::getAddDeviceMessage(Dev& device, QString newDevDescription) {
  QByteArray msg = get_message("247", device);
  try {
    QByteArray description = Utils::qstringnumbersToQByteArray(newDevDescription);
    msg[4] = description[4]; //floor
    msg[5] = description[5]; //room
    msg[6] = description[6]; //group -Lamp
    msg[9] = description[0]; //body1 -ip1
    msg[10] = description[1]; //body2 -ip2
    msg[11] = description[2]; //body3 -ip3
    msg[12] = description[3]; //body4 -ip4
    return msg;
  } catch(...) {
    msg[4] = Utils::qstringToQuint8("1"); //floor
    msg[5] = Utils::qstringToQuint8("1"); //room
    msg[6] = Utils::qstringToQuint8("1"); //group
    msg[9] = Utils::qstringToQuint8("127"); //body1 -ip1
    msg[10] = Utils::qstringToQuint8("0"); //body2 -ip2
    msg[11] = Utils::qstringToQuint8("0"); //body3 -ip3
    msg[12] = Utils::qstringToQuint8("1"); //body4 -ip4
    return msg;
  }
}

QByteArray Messages::getRemoveDeviceMessage(Dev& device, QString id) {
  QByteArray msg = get_message("248", device);
  try {
    QByteArray ids = Utils::qstringnumbersToQByteArray(id);
    msg[9] = ids[0]; //body1 - deviceIdHigh
    msg[10] = ids[1]; //body2 -deviceIdLow
    return msg;
  } catch(...) {
    msg[9] = Utils::qstringToQuint8("0"); //body1 - deviceIdHigh
    msg[10] = Utils::qstringToQuint8("1"); //body2 - deviceIdLow
    return msg;
  }
}
