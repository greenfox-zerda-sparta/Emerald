#include "consolereader.h"
#include <QDebug>
#include <QEventLoop>
#include <QCoreApplication>

ConsoleReader::ConsoleReader(QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(run()), this, SLOT(onRun()));
    qDebug() << "Enter a command(/command) or a message(message)";
    qDebug() << "   Commands:   connect         - start tcp connection";
    qDebug() << "               quit            - quit program";
    //qDebug() << "               openudp         - open udp socket to autoconnection";
    qDebug() << "               closeudp        - close udp socket";
    qDebug() << "               ip=10.28.2.150  - set ip";
    qDebug() << "               port=4321       - set tcp port";
    qDebug() << "               id=12121212     - set device id";
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
    QDebug qdebug = qDebug();
    qdebug.noquote() << message;
}
