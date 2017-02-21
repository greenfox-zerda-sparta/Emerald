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
  serverAddress = "localhost";
//    serverAddress = "10.27.6.64";
  datagramNeeded = "turquoise&emerald";
  cReader = new ConsoleReader();
  consoleThread = new QThread();
  cReader->moveToThread(consoleThread);
  broadcastReceiver = new BroadcastSocket(datagramNeeded);
  isTcpOn = false;
  isUdpOn = false;
  isDevOn = false;
  connect(socket, SIGNAL(readyRead()), this, SLOT(newDataAvailable()));
  connect(socket, SIGNAL(connected()), this, SLOT(trackConnectedState()));
  connect(this, SIGNAL(incomingMessage(QString)), cReader, SLOT(writeToConsole(QString)), Qt::DirectConnection);
  connect(this, SIGNAL(incomingMessage(QString)), this, SLOT(reactToIncomingMessage(QString)));
  connect(this, SIGNAL(write(QString)), cReader, SLOT(writeToConsole(QString)), Qt::DirectConnection);
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
  emit incomingMessage(Utils::messageToNumbers(incomingData));
}

void DummyClient::trackConnectedState() {
  emit write("   Connected.");
}

void DummyClient::reactToIncomingMessage(QString message) {
  if (isDevOn) {
      QByteArray msg = messGetter.getNextMessage(message,me);
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
    startCommand("connect");
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
    helpMessage += "               add                 - send 'add device' command\n";
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
    isUdpOn = ! isUdpOn;
      if(isUdpOn) {
          emit manualStartUDP();
      } else {
          emit manualCloseUDP();
          emit write("   UDP Socket is closed.");
      }
  } else  if (text == "tcp") {
    isTcpOn = !isTcpOn;
    if (socket->state() == QTcpSocket::ConnectedState) {
      Disconnect();
    }
    if(isTcpOn) {
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
  } else if (text == "srs") {
    QByteArray msg = messGetter.get_message(text, me);
    sendMessage(msg);
    emit write("   RESTART SERVER command is sent.");
  } else if (text == "sre") {
    QByteArray msg = messGetter.get_message(text, me);
    sendMessage(msg);
    emit write("   RESET SERVER command is sent.");
  } else if (text == "add") {
    QByteArray msg = messGetter.get_message(text, me);
    sendMessage(msg);
    emit write("   ADD DEVICE COMMAND is sent.");
  } else if (text == "ack") {
    QByteArray msg = messGetter.get_message(text, me);
    sendMessage(msg);
    emit write("   ACK message is sent.");
  } else if (text == "crc") {
    QByteArray msg = messGetter.get_message(text, me);
    sendMessage(msg);
    emit write("   CRC error message is sent.");
  } else if (text == "suc") {
    QByteArray msg = messGetter.get_message(text, me);
    sendMessage(msg);
    emit write("   SUCCESS message is sent.");
  } else if (text == "err") {
    QByteArray msg = messGetter.get_message(text, me);
    sendMessage(msg);
    emit write("   ERROR message is sent.");
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
