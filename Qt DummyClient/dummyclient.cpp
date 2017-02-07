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
    deviceId = "00000000";
    serverPort = 1234;
    serverAddress = "T-Pc";
//    serverAddress = "10.27.6.126";
    datagramNeeded = "turquoise&emerald";
    cReader = new ConsoleReader();
    consoleThread = new QThread();
    cReader->moveToThread(consoleThread);
    broadcastReceiver = new BroadcastSocket(datagramNeeded);
    isEcho = true;
    connect(socket, SIGNAL(readyRead()), this, SLOT(newDataAvailable()));
    connect(socket, SIGNAL(connected()), this, SLOT(sendFirstMessage()));

    connect(this, SIGNAL(incomingMessage(QString)), cReader, SLOT(writeToConsole(QString)), Qt::DirectConnection);
    connect(this, SIGNAL(incomingMessage(QString)), this, SLOT(echo(QString)));

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


void DummyClient::run()
{
    consoleThread->start();
    emit runConsole();
    QString message;
    message = "   Server Ip: " + serverAddress;
    message += ", Server Port: " + QString::number(serverPort);
    message += ", ID: " + deviceId;
    emit write(message);
    emit write("   Echo is on.");
}

void DummyClient::connectToServer()
{
    if (socket->state() == QTcpSocket::UnconnectedState) {
        emit write("   Connecting to host...");
        socket->connectToHost(serverAddress, serverPort);
    }
}

void DummyClient::newDataAvailable()
{
    QByteArray incomingData = (socket->readAll()).trimmed();
    QString str(QString::fromUtf8(incomingData));
    emit incomingMessage(str);
}

void DummyClient::sendFirstMessage()
{
    emit write("   Connected.");
    sendMessage(deviceId);
}

void DummyClient::echo(QString message)
{
    if(isEcho){sendMessage(message);}
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

/*
 void DummyClient::timerEvent(QTimerEvent *)
{
    if (socket->state() == QTcpSocket::UnconnectedState) {
        emit write("   Connecting to host...");
        connectToServer();
    }
}
*/

void DummyClient::Disconnect()
{
    socket->disconnectFromHost();
    emit write("   TCP connection is closed.");
}

void DummyClient::closeSocket()
{
    socket->close();
}

void DummyClient::parseInputFromCommandLine(QString text)
{
    if(text.left(1) == "/")
    {
        startCommand(text.mid(1));
    }
    else if(text == datagramNeeded)
    {
        startCommand("connect");
    }
    else
    {
        sendMessage(text);
    }
}

void DummyClient::startCommand(QString text)
{
    text = text.toLower();
    if (text == "connect")
    {
        if (socket->state() == QTcpSocket::ConnectedState)
        {
            Disconnect();
        }
        connectToServer();
    }
    else if (text == "disconnect")
    {
        if (socket->state() == QTcpSocket::ConnectedState)
        {
            Disconnect();
        }
    }
    else if (text == "echo")
    {
        isEcho = true;
        emit write("   Echo is on.");
    }
    else if (text == "noecho")
    {
        isEcho = false;
        emit write("   Echo is off.");
    }
    else if (text == "quit")
    {
        emit write("   Bye!");
        delete broadcastReceiver;
        emit quit();
    }
    else if (text.left(6) == "setip=")
    {
        serverAddress = text.mid(6);
        emit write("   server address: " + serverAddress);
    }
    else if (text.left(8) == "setport=")
    {
        serverPort = qstringToQuint32(text.mid(8));
        emit write("   server port: " + QString::number(serverPort));

    }
    else if (text.left(6) == "setid=")
    {
        deviceId = text.mid(6);
        emit write("   Device ID: " + deviceId);
    }
    else if (text == "udpauto")
    {
        emit manualStartUDP();
//        emit write("   UDP Socket is opened.");
    }
    else if (text == "udpstop")
    {
        emit manualCloseUDP();
        emit write("   UDP Socket is in manual mode.");
    }
    else
    {
        emit write("   Invalid command.");
    }
}

void DummyClient::Quit()
{
    if (socket->state() == QTcpSocket::ConnectedState)
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
