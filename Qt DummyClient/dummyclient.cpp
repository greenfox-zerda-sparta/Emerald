#include "dummyclient.h"
#include <QTcpSocket>
#include <QTimer>
#include <QDebug>
#include <iostream>
#include <QThread>
#include <QTextStream>

DummyClient::DummyClient(QObject* parent) : QObject(parent) {
  socket = new QTcpSocket(this);
  deviceId = "ui";
  changeDev();
  serverPort = 1234;
//  serverAddress = "localhost";
  serverAddress = "10.27.6.206";
  datagramNeeded = "turquoise&emerald";
  cReader = new ConsoleReader();
  consoleThread = new QThread();
  cReader->moveToThread(consoleThread);
  broadcastReceiver = new BroadcastSocket(datagramNeeded);
  isUdpOn = false;
  isDevOn = false;
  connect(socket, SIGNAL(readyRead()), this, SLOT(newDataAvailable()));
  connect(socket, SIGNAL(connected()), this, SLOT(trackConnectedState()));
  connect(this, SIGNAL(incomingMessage(QString)), cReader, SLOT(writeToConsole(QString)), Qt::DirectConnection);
  connect(this, SIGNAL(incomingMessage(QString)), this, SLOT(reactToIncomingMessage(QString)));
  connect(this, SIGNAL(write(QString)), cReader, SLOT(writeToConsole(QString)), Qt::DirectConnection);
  connect(this, SIGNAL(setConsoleReaderCommandMode(int)), cReader, SLOT(setCommandMode(int)), Qt::DirectConnection);
  connect(cReader, SIGNAL(toAddCommand(QString)), this, SLOT(addDevice(QString)), Qt::DirectConnection);
  connect(cReader, SIGNAL(toRemoveCommand(QString)), this, SLOT(removeDevice(QString)), Qt::DirectConnection);
  connect(cReader, SIGNAL(toSetCommand(QString)), this, SLOT(setDevice(QString)), Qt::DirectConnection);
  connect(cReader, SIGNAL(inputFromCommandLine(QString)), this, SLOT(parseInputFromCommandLine(QString)));
  connect(consoleThread, SIGNAL(finished()), cReader, SLOT(deleteLater()));
  connect(consoleThread, SIGNAL(finished()), consoleThread, SLOT(deleteLater()));
  connect(this, SIGNAL(runConsole()), cReader, SLOT(onRun()));
  //    connect(cReader, SIGNAL(finished()), consoleThread, SLOT(quit()));
  connect(this, SIGNAL(quit()), consoleThread, SLOT(quit()), Qt::DirectConnection);
  // UDP socket control
  connect(broadcastReceiver, SIGNAL(write(QString)), cReader, SLOT(writeToConsole(QString)), Qt::DirectConnection);
  connect(socket, SIGNAL(connected()), broadcastReceiver, SLOT(close()));
  connect(socket, SIGNAL(disconnected()), broadcastReceiver, SLOT(startUDP()));
  connect(this, SIGNAL(manualStartUDP()), broadcastReceiver, SLOT(manualStart()));
  connect(this, SIGNAL(manualCloseUDP()), broadcastReceiver, SLOT(manualClose()));
//    connect(broadcastReceiver, SIGNAL(newDatagram(QString)), cReader, SLOT(writeToConsole(QString)), Qt::DirectConnection);
  connect(broadcastReceiver, SIGNAL(newDatagram(QString)), this, SLOT(parseInputFromCommandLine(QString)));
  addDeviceMessages = {"Enter the Ip separated by space /do not use points/: ",
                       "Enter the floor number: ",
                       "Enter the room number: ",
                       "Choose the device group id: "
                      };
  availableDevices = "\n1 - Lamp\n";
  availableDevices += "2 - Heating\n";
  availableDevices += "3 - Cooling\n";
  availableDevices += "4 - Blinds\n";
  availableDevices += "5 - Alarm\n";
  availableDevices += "6 - Door\n";
  availableDevices += "7 - Water consumption\n";
  availableDevices += "8 - Current consumption in month\n";
  availableDevices += "9 - Current consumption sum";
  availableRooms = "\n1 - garage\n";
  availableRooms += "2 - workshop\n";
  availableRooms += "3 - kitchen\n";
  availableRooms += "4 - living room\n";
  availableRooms += "5 - half bedroom\n";
  availableRooms += "6 - north bedroom\n";
  availableRooms += "7 - south bedroom\n";
  availableRooms += "8 - kidroom\n";
  availableRooms += "9 - bathroom";
  newDeviceDescription = "";
  availableFloors = "\n1 - basement\n";
  availableFloors += "2 - ground floor\n";
  availableFloors += "3 - first floor";
}


void DummyClient::run() {
  consoleThread->start();
  printHelp();
  emit runConsole();
  QString message;
  message = "   Server Ip: " + serverAddress;
  message += ", Server Port: " + QString::number(serverPort);
  message += ", Device: " + deviceId;
  emit write(message);
  emit write("   Automatic device simulation is off.");
  emit write("   UDP Socket is closed.");
}

void DummyClient::connectToServer() {
  if (socket->state() == QTcpSocket::UnconnectedState) {
    emit write("   Connecting to host...");
    socket->connectToHost(serverAddress, serverPort);
  }
}

void DummyClient::newDataAvailable() {
  QByteArray incomingData = (socket->readAll()).trimmed();
//    QString str(QString::fromUtf8(incomingData));
  emit write("   Incoming message:");
  printWhichMessage(incomingData);
  emit incomingMessage(Utils::messageToNumbers(incomingData));
}

void DummyClient::trackConnectedState() {
  emit write("   Connected.");
}

void DummyClient::reactToIncomingMessage(QString message) {
  if (isDevOn) {
    QByteArray msg = messGetter.getNextMessage(message,me);
    emit write("   Auto-reply:");
    printWhichMessage(msg);
    emit write(Utils::messageToNumbers(msg));
    sendMessage(msg);
  }
}

void DummyClient::sendMessage(QString message) {
  if (socket->state() != QTcpSocket::ConnectedState) {
    return;
  }
//    message += "\n";
  QByteArray temp = Utils::qstringnumbersToQByteArray(message);
  temp.append(10);
  socket->write(temp);
  socket->flush();
}

void DummyClient::sendMessage(QByteArray message) {
  if (socket->state() != QTcpSocket::ConnectedState) {
    return;
  }
  message.append("\n");
  socket->write(message);
  socket->flush();
}

void DummyClient::Disconnect() {
  socket->disconnectFromHost();
  emit write("   TCP connection is closed.");
}

void DummyClient::closeSocket() {
  socket->close();
}

void DummyClient::parseInputFromCommandLine(QString text) {
  if(text.left(1) == "/") {
    startCommand(text.mid(1));
  } else if(text == datagramNeeded) {
    startCommand("tcp");
  } else {
    sendMessage(text);
  }
}

void DummyClient::printHelp() {
  QString helpMessage ;
  helpMessage = "Enter a command(/command) or a message(message) to send\n";
  helpMessage += "   Commands available:\n";
  helpMessage += "               h                   - print this help\n";
  helpMessage += "               q                   - quit program\n";
  helpMessage += "               setip=10.28.2.150   - set server ip\n";
  helpMessage += "               setport=4321        - set tcp port\n";
  helpMessage += "               udp                 - toggling modes auto-/manual connection\n";
  helpMessage += "               tcp                 - toggling tcp connect/disconnect\n";
  helpMessage += "               dev                 - toggling automatic device simulation/UI\n";
  helpMessage += "               sst                 - send 'Stop server' command\n";
  helpMessage += "               srs                 - send 'Restart server' command\n";
  helpMessage += "               sre                 - send 'Reset server' command\n";
  helpMessage += "               cst                 - send 'get status report' command\n";
  helpMessage += "               add                 - send 'add device' command\n";
  helpMessage += "               rem                 - send 'remove device' command\n";
  helpMessage += "               set                 - send 'set device' command\n";
  helpMessage += "               ack                 - send 'ack' message\n";
  helpMessage += "               crc                 - send 'crc error' message\n";
  helpMessage += "               suc                 - send 'success' message\n";
  helpMessage += "               err                 - send 'error in work' message\n";
  emit write(helpMessage);
}

void DummyClient::startCommand(QString text) {
  text = text.toLower();
  if (text == "h") {
    printHelp();
  } else if (text == "q") {
    emit write("   Bye!");
    delete broadcastReceiver;
    emit quit();
  } else if (text.left(6) == "setip=") {
    serverAddress = text.mid(6);
    emit write("   server address: " + serverAddress);
  } else if (text.left(8) == "setport=") {
    serverPort = qstringToQuint32(text.mid(8));
    emit write("   server port: " + QString::number(serverPort));
  } else if (text == "udp") {
    isUdpOn = !isUdpOn;
    if(isUdpOn) {
      emit write("   Autodetection is switched on.");
      emit manualStartUDP();
    } else {
      emit write("   Autodetection is switched off.");
      emit manualCloseUDP();
    }
  } else  if (text == "tcp") {
    if (socket->state() == QTcpSocket::ConnectedState) {
      Disconnect();
    } else {
      connectToServer();
    }
  } else if (text == "dev") {
    isDevOn = !isDevOn;
    QString toDisp = "   Automatic device simulation is ";
    toDisp += (isDevOn?"on.":"off.");
    emit write(toDisp);
    if (!isDevOn) {
      deviceId = "ui";
      changeDev();
      emit write("   Device: " + deviceId);
    }
  } else if (text == "sst") {
    QByteArray msg = messGetter.get_message(text, me);
    sendMessage(msg);
    emit write("   STOP SERVER command is sent.");
    emit write(Utils::messageToNumbers(msg));
  } else if (text == "srs") {
    QByteArray msg = messGetter.get_message(text, me);
    sendMessage(msg);
    emit write("   RESTART SERVER command is sent.");
    emit write(Utils::messageToNumbers(msg));
  } else if (text == "sre") {
    QByteArray msg = messGetter.get_message(text, me);
    sendMessage(msg);
    emit write("   RESET SERVER command is sent.");
    emit write(Utils::messageToNumbers(msg));
  } else if (text == "cst") {
    QByteArray msg = messGetter.get_message(text, me);
    sendMessage(msg);
    emit write("   GET STATUS REPORT command is sent.");
    emit write(Utils::messageToNumbers(msg));
  } else if (text == "add") {
    addDevice();
  } else if (text == "rem") {
    removeDevice();
  } else if (text == "set") {
    setDevice();
  } else if (text == "ack") {
    QByteArray msg = messGetter.get_message(text, me);
    sendMessage(msg);
    emit write("   ACK message is sent.");
    emit write(Utils::messageToNumbers(msg));
  } else if (text == "crc") {
    QByteArray msg = messGetter.get_message(text, me);
    sendMessage(msg);
    emit write("   CRC error message is sent.");
    emit write(Utils::messageToNumbers(msg));
  } else if (text == "suc") {
    QByteArray msg = messGetter.get_message(text, me);
    sendMessage(msg);
    emit write("   SUCCESS message is sent.");
    emit write(Utils::messageToNumbers(msg));
  } else if (text == "err") {
    QByteArray msg = messGetter.get_message(text, me);
    sendMessage(msg);
    emit write("   ERROR message is sent.");
    emit write(Utils::messageToNumbers(msg));
  } else {
    emit write("   Invalid command.");
  }
}

void DummyClient::Quit() {
  if (socket->state() == QTcpSocket::ConnectedState) {
    Disconnect();
  }
  closeSocket();
  emit quit();
}

quint32 DummyClient::qstringToQuint32(QString string) {
  QTextStream ts(&string);
  quint32 result = 0;
  ts >> result;
  return result;
}

void DummyClient::changeDev() {
  if(deviceId == "ui") {
    me.deviceIdHigh = Utils::qstringToQuint8("255");
    me.deviceIdLow = Utils::qstringToQuint8("253");
    me.homeId = Utils::qstringToQuint8("255");
    me.floorId = Utils::qstringToQuint8("255");
    me.roomId = Utils::qstringToQuint8("255");
    me.groupId = Utils::qstringToQuint8("255");
    me.status = Utils::qstringToQuint8("100");
  }
}

void DummyClient::addDevice(QString newDevDescription) {
  newDeviceDescription = newDeviceDescription + " ";
  newDevDescription = newDeviceDescription + newDevDescription;
  newDevDescription = newDevDescription.trimmed();
  QStringList list = newDevDescription.split(' ');
  int index = list.size() - 1;
  bool isError = false;
  if(QString::number(Utils::qstringToQuint8(list[list.size() -1])) != list[list.size() -1] && list.last() != "") {
    isError = true;
  }
  if (index < 3) {
    emit setConsoleReaderCommandMode(1);
    emit write(addDeviceMessages[0]);
    return;
  } else if (index == 3) {
    if (isError) {
      index = 0;
      emit write("   Invalid value. ");
    } else {
      newDeviceDescription = newDevDescription;
      index = 1;
      emit write(availableFloors);
    }
    emit write(addDeviceMessages[index]);
    return;
  } else if(index < 6) {
    index -= 2;
    if (isError) {
      index -= 1;
      emit write("   Invalid value. ");
    }
    if(index == 2) {
      emit write(availableRooms);
    }
    if(index == 3) {
      emit write(availableDevices);
    }
    emit write(addDeviceMessages[index]);
    if(!isError) {
      newDeviceDescription = newDevDescription;
    }
    return;
  }
  if(index > 5) {
    if(isError) {
      emit write(availableDevices);
      emit write(addDeviceMessages[index-2]);
      return;
    }
    newDeviceDescription = "";
    QByteArray msg = messGetter.getAddDeviceMessage(me, newDevDescription);
    sendMessage(msg);
    emit setConsoleReaderCommandMode(0);
    emit write("   ADD DEVICE COMMAND is sent.");
    emit write(Utils::messageToNumbers(msg));
  }
}

void DummyClient::removeDevice(QString id) {
  int index = 0;
  bool isError = false;
  if(id != "") {
    id = id.trimmed();
    QStringList list = id.split(' ');
    index = list.size() - 1;
    if((QString::number(Utils::qstringToQuint8(list[list.size() -1])) != list[list.size() -1] && list.last() != "") &&
        (QString::number(Utils::qstringToQuint8(list[0])) != list[0] && list.last() != "")) {
      isError = true;
    }
  }
  if(index > 0) {
    if (isError) {
      index = 1;
      emit write("   Invalid value.");
    } else {
      QByteArray msg = messGetter.getRemoveDeviceMessage(me, id);
      sendMessage(msg);
      emit setConsoleReaderCommandMode(0);
      emit write("   REMOVE DEVICE COMMAND is sent.");
      emit write(Utils::messageToNumbers(msg));
      return;
    }
  }
  if(index < 1) {
    emit setConsoleReaderCommandMode(2);
  }
  emit write("Enter Device ID High and Low separated by space: ");
}

void DummyClient::setDevice(QString which) {
  emit setConsoleReaderCommandMode(3);
  int index = 0;
  newDeviceDescription = newDeviceDescription + " ";
  which = (newDeviceDescription + which).trimmed();
  if(which != "") {
    QStringList list = which.split(' ');
    index = list.size();
  }
  if (index < 3) {
    emit write("Enter Device ID High, Low and the new value separated by space /floor, room, group optional/: ");
  } else {
      QByteArray msg = messGetter.getSetDeviceMessage(me, which);
      sendMessage(msg);
      emit setConsoleReaderCommandMode(0);
      QString toConsole = "   SET DEVICE COMMAND is sent with value: ";
      toConsole += QString::number(msg[9]);
      emit write(toConsole);
      emit write(Utils::messageToNumbers(msg));
      return;
  }
}

void DummyClient::printWhichMessage(QByteArray msg) {
  if(msg[2] == (char)0) {
    emit write("   OFF command");
  }
  if(msg[2] == (char)1) {
    emit write("   ON command");
  }
  if(msg[2] == (char)2) {
    emit write("   STANDBY command");
  }
  if(msg[2] == (char)3) {
    emit write("   SET() command");
  }
  if(msg[2] == (char)4) {
    emit write("   SET+() command");
  }
  if(msg[2] == (char)5) {
    emit write("   SET-() command");
  }
  if(msg[2] == (char)239) {
    emit write("   SET DATA command");
  }
  if(msg[2] == (char)240) {
    emit write("   COMMAND REPLY - ERROR message");
  }
  if(msg[2] == (char)241) {
    emit write("   COMMAND REPLY - SUCCESS message");
  }
  if(msg[2] == (char)242) {
    emit write("   STATUS REPORT message");
  }
  if(msg[2] == (char)246) {
    emit write("   GET STATUS REPORT command");
  }
  if(msg[2] == (char)247) {
    emit write("   ADD DEVICE command");
  }
  if(msg[2] == (char)248) {
    emit write("   REMOVE DEVICE command");
  }
  if(msg[2] == (char)249) {
    emit write("   DEVICE DISCONNECTED message");
  }
  if(msg[2] == (char)250) {
    emit write("   NO ANSWER/DEVICE ERROR message");
  }
  if(msg[2] == (char)251) {
    emit write("   CRC ERROR message");
  }
  if(msg[2] == (char)252) {
    emit write("   ACK message");
  }
  if(msg[2] == (char)253) {
    emit write("   RESET SERVER command");
  }
  if(msg[2] == (char)254) {
    emit write("   RESTART SERVER command");
  }
  if(msg[2] == (char)255) {
    emit write("   STOP SERVER command");
  }
}
