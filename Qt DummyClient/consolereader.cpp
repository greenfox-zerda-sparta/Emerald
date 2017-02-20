#include "consolereader.h"
#include <QDebug>
#include <QEventLoop>
#include <QCoreApplication>

ConsoleReader::ConsoleReader(QObject* parent) : QObject(parent) {
  connect(this, SIGNAL(run()), this, SLOT(onRun()));
  qDebug() << "Enter a command(/command) or a message(message)";
  qDebug() << "   Commands:   connect             - start tcp connection";
  qDebug() << "               disconnect          - close tcp connection";
  qDebug() << "               udpauto             - start udp auto up- & reconnection";
  qDebug() << "               udpstop             - stop udp autoconnection";
  qDebug() << "               noecho              - stop echoing back to server";
  qDebug() << "               echo                - echoing messages back to server";
  qDebug() << "               setip=10.28.2.150   - set ip";
  qDebug() << "               setport=4321        - set tcp port";
  qDebug() << "               setdev=ui/lamp      - set mock device";
  qDebug() << "               1                   - Stop server";
  qDebug() << "               2                   - Restart server";
  qDebug() << "               3                   - Reset server";
  qDebug() << "               ack                 - send 'ok' message";
  qDebug() << "               crc                 - send 'crc error' message";
  qDebug() << "               suc                 - send 'success' message";
  qDebug() << "               err                 - send 'error in work' message";
  qDebug() << "               add                 - send 'add device' message";
}

void ConsoleReader::onRun() {
  QCoreApplication::processEvents();
  readCommand();
}

void ConsoleReader::readCommand() {
  std::string line;
  std::getline(std::cin, line);
  if (std::cin.eof() || line == "quit") {
    emit finished();
  } else {
    emit inputFromCommandLine(QString::fromStdString(line));
    emit run();
  }
}

void ConsoleReader::writeToConsole(QString message) {
  QDebug qdebug = qDebug();
  qdebug.noquote() << message;
}
