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
    deviceId = "12345678";
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
    connect(socket, SIGNAL(connected()), broadcastReceiver, SLOT(close()));
    connect(socket, SIGNAL(disconnected()), broadcastReceiver, SLOT(startUDP()));
//    connect(this, SIGNAL(openUdpSocket()), broadcastReceiver, SLOT(startUDP()));
    connect(this, SIGNAL(closeUdpSocket()), broadcastReceiver, SLOT(close()));
    connect(broadcastReceiver, SIGNAL(newDatagram(QString)), cReader, SLOT(writeToConsole(QString)), Qt::DirectConnection);
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
    emit write("   Connected.");
    StopTimer();
    sendMessage(deviceId);
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
        emit write("   Connecting to host...");
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
    if(text.left(1) == "/")
    {
        startCommand(text.mid(1));
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
        if (timerId != -1)
        {
            Disconnect();
        }
        StartTimer();
    }
    else if (text == "quit")
    {
        emit write("   Bye!");
        delete broadcastReceiver;
        emit quit();
    }
    else if (text.left(3) == "ip=")
    {
        serverAddress = text.mid(3);
        emit write("   server address: " + serverAddress);
    }
    else if (text.left(5) == "port=")
    {
        serverPort = qstringToQuint32(text.mid(5));
        emit write("   server port: " + QString::number(serverPort));

    }
    else if (text.left(3) == "id=")
    {
        deviceId = text.mid(3);
        emit write("   Device ID: " + deviceId);
    }
    //else if (text == "openudp")
    //{
        //emit openUdpSocket();
        //emit write("   UDP Socket is opened.");
    //}
    else if (text == "closeudp")
    {
        emit closeUdpSocket();
        emit write("   UDP Socket is closed.");
    }
    else
    {
        emit write("   Invalid command.");
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
