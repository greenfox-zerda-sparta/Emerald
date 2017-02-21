#include "consolereader.h"
#include <QDebug>
#include <QEventLoop>
#include <QCoreApplication>

ConsoleReader::ConsoleReader(QObject* parent) : QObject(parent) {
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
  } else {
    emit inputFromCommandLine(QString::fromStdString(line));
    emit run();
  }
}

void ConsoleReader::writeToConsole(QString message) {
  QDebug qdebug = qDebug();
  qdebug.noquote() << message;
}
