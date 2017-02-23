#include "consolereader.h"
#include <QDebug>
#include <QEventLoop>
#include <QCoreApplication>

ConsoleReader::ConsoleReader(QObject* parent) : QObject(parent) {
  isCommandMode = 0;
  connect(this, SIGNAL(run()), this, SLOT(onRun()));
}

void ConsoleReader::onRun() {
  QCoreApplication::processEvents();
  readCommand();
}

void ConsoleReader::readCommand() {
  std::string line;
  std::getline(std::cin, line);
  if (std::cin.eof() || line == "q") {
    emit finished();
  } else if(isCommandMode == 0) {
    emit inputFromCommandLine(QString::fromStdString(line));
    emit run();
  } else if(isCommandMode == 1) {
    emit toAddCommand(QString::fromStdString(line));
    emit run();
  } else if(isCommandMode == 2) {
    emit toRemoveCommand(QString::fromStdString(line));
    emit run();
  }
}

void ConsoleReader::writeToConsole(QString message) {
  QDebug qdebug = qDebug();
  qdebug.noquote() << message;
}

void ConsoleReader::setCommandMode(int mode) {
  isCommandMode = mode;
}
