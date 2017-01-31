#include "dummyclient.h"
#include <QTcpSocket>
#include <QTimer>
#include <QDebug>
#include <iostream>
#include <QThread>
#include <QTextStream>

DummyClient::DummyClient(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    gadgetId = "12345678";
    serverPort = 1234;
//    serverAddress = "T-Pc";
    serverAddress = "10.27.6.126";
    timerId = -1;
    cReader = new ConsoleReader();
    consoleThread = new QThread();
    cReader->moveToThread(consoleThread);
    broadcastReceiver = new BroadcastSocket();
    connect(socket, SIGNAL(readyRead()), this, SLOT(newDataAvailable()));
    connect(socket, SIGNAL(connected()), this, SLOT(sendFirstMessage()));
    connect(this, SIGNAL(incomingMessage(QString)), cReader, SLOT(writeToConsole(QString)), Qt::DirectConnection);

    connect(this, SIGNAL(incomingMessage(QString)), this, SLOT(sendMessage(QString)), Qt::DirectConnection);
    connect(this, SIGNAL(write(QString)), cReader, SLOT(writeToConsole(QString)), Qt::DirectConnection);
    connect(cReader, SIGNAL(inputFromCommandLine(QString)), this, SLOT(parseInputFromCommandLine(QString)));
    connect(consoleThread, SIGNAL(finished()), cReader, SLOT(deleteLater()));
    connect(consoleThread, SIGNAL(finished()), consoleThread, SLOT(deleteLater()));
    connect(this, SIGNAL(runConsole()), cReader, SLOT(onRun()));
    //    connect(cReader, SIGNAL(finished()), consoleThread, SLOT(quit()));
    connect(this, SIGNAL(quit()), consoleThread, SLOT(quit()), Qt::DirectConnection);
    // UDP socket control
    connect(broadcastReceiver, SIGNAL(newDatagram(QString)), consoleThread, SLOT(writeToConsole(QString)), Qt::DirectConnection);
    connect(this, SIGNAL(closeUdpSocket()), broadcastReceiver, SLOT(close()), Qt::DirectConnection);
}


void DummyClient::run()
{
    consoleThread->start();
    emit runConsole();
    QString message;
    message = "Server Ip: " + serverAddress;
    message += ", Server Port: " + QString::number(serverPort);
    message += ", ID: " + gadgetId;
    emit write(message);
}

void DummyClient::StartTimer()
{
    timerId = startTimer(200);

}

void DummyClient::StopTimer()
{
    if(!(timerId == -1))
    {
        killTimer(timerId);
        timerId = -1;
    }
}

void DummyClient::connectToServer()
{
    socket->connectToHost(serverAddress, serverPort);
}

void DummyClient::newDataAvailable()
{
    QByteArray incomingData = (socket->readAll()).trimmed();
    QString str(QString::fromUtf8(incomingData));
    emit incomingMessage(str);
}

void DummyClient::sendFirstMessage()
{
    emit write("Connected.");
    sendMessage(gadgetId);
}

void DummyClient::sendMessage(QString message)
{
    if (socket->state() != QTcpSocket::ConnectedState) {
        return;
    }
    message += "\n";
    socket->write(message.toUtf8());
    socket->flush();
}

void DummyClient::timerEvent(QTimerEvent *)
{
    if (socket->state() == QTcpSocket::UnconnectedState) {
        emit write("Connecting to host...");
        connectToServer();
    }
}

void DummyClient::Disconnect()
{
    StopTimer();
    socket->disconnectFromHost();
}

void DummyClient::closeSocket()
{
    socket->close();
}

void DummyClient::parseInputFromCommandLine(QString text)
{
    if(text.left(2) == "c:")
    {
        startCommand(text.mid(2));
    }
    else if(text.left(2) == "s:")
    {
        sendMessage(text.mid(2));
    }
    else
    {
        emit write("Invalid argument");
    }
}

void DummyClient::startCommand(QString text)
{
    if (text == "CONNECT" || text == "connect")
    {
        if (timerId != -1)
        {
            Disconnect();
        }
        StartTimer();
    }
    else if (text == "QUIT" || text == "quit")
    {
        emit write("Bye!");
        emit quit();
    }
    else if (text.left(3) == "IP " || text.left(3) == "ip ")
    {
        serverAddress = text.mid(3);
        emit write("  server address: " + serverAddress);
    }
    else if (text.left(5) == "PORT " || text.left(5) == "port ")
    {
        serverPort = qstringToQuint32(text.mid(5));
        emit write("  server port: " + QString::number(serverPort));

    }
    else if (text.left(3) == "ID " || text.left(3) == "id ")
    {
        gadgetId = text.mid(3);
        emit write("  Gadget ID: " + gadgetId);
    }
    else if (text.left(3) == "CLOSEUDP " || text.left(3) == "closeudp ")
    {
        emit closeUdpSocket();
        emit write("  UDP Socket is closed.");
    }
    else
    {
        emit write("Invalid command.");
    }
}

void DummyClient::Quit()
{
    if (timerId != -1)
    {
        Disconnect();
    }
    closeSocket();
    emit quit();
}

quint32 DummyClient::qstringToQuint32(QString string)
{
    QTextStream ts(&string);
    quint32 result = 0;
    ts >> result;
    return result;

}
