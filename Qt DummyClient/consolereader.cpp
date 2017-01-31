#include "consolereader.h"
#include <QDebug>
#include <QEventLoop>
#include <QCoreApplication>

ConsoleReader::ConsoleReader(QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(run()), this, SLOT(onRun()));
    qDebug() << "Enter a command(c:COMMAND) or a message(s:message)";
    qDebug() << "   Commands:  CONNECT";
    qDebug() << "               QUIT";
    qDebug() << "               IP 10.28.2.150";
    qDebug() << "               PORT 4321";
    qDebug() << "               ID 12121212";
}

void ConsoleReader::onRun()
{
    QCoreApplication::processEvents();
    readCommand();
}

void ConsoleReader::readCommand()
{
    std::string line;
    std::getline(std::cin, line);
    if (std::cin.eof() || line == "quit") {
        emit finished();
    } else {
        emit inputFromCommandLine(QString::fromStdString(line));
        emit run();
    }
}

void ConsoleReader::writeToConsole(QString message)
{
    qDebug() << message;
}
