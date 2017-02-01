#include "clientsocket.h"

#include <QTcpSocket>
#include <QTimer>

ClientSocket::ClientSocket(QObject *parent)
{
    socket = new QTcpSocket(this);
    serverPort = 11000;
    serverAddress = "T-Pc";
    userName = "Feri";
    timerId = -1;
    isFirstMessageSent = false;
    connect(socket, SIGNAL(readyRead()), this, SLOT(newDataAvailable()));
    connect(parent, SIGNAL(sendMessage(QString)), this, SLOT(onSend(QString)));
    connect(socket, SIGNAL(disconnected()), this, SLOT(closeSocket()));
}

void ClientSocket::StartTimer()
{
    timerId = startTimer(200);

}

void ClientSocket::StopTimer()
{
    if(!(timerId == -1))
    {
        killTimer(timerId);
        timerId = -1;
    }
}

ClientSocket::~ClientSocket()
{
    disconnect(socket, NULL, this, NULL);
}

void ClientSocket::connectToServer()
{
    emit incomingMessage("Connecting to host...");
    socket->connectToHost(serverAddress, serverPort);
}

void ClientSocket::newDataAvailable()
{
    incomingData = (socket->readAll()).trimmed();
    QString str(QString::fromUtf8(incomingData));
    emit incomingMessage(str);
    if(!isFirstMessageSent)
    {
        onSend(userName);
        isFirstMessageSent = true;
    }

}

void ClientSocket::onSend(QString message)
{
    if (socket->state() != QTcpSocket::ConnectedState) {
        return;
    }
    message += "\n";
    socket->write(message.toUtf8());
    socket->flush();
}

void ClientSocket::timerEvent(QTimerEvent *)
{
    if (socket->state() == QTcpSocket::UnconnectedState) {
        connectToServer();
    }
}

void ClientSocket::Disconnect()
{
    socket->disconnectFromHost();
    isFirstMessageSent = false;
}

void ClientSocket::closeSocket()
{
    socket->close();
}
