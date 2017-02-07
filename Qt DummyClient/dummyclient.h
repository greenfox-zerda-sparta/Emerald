#ifndef DUMMYCLIENT_H
#define DUMMYCLIENT_H

#include <QHostAddress>
#include <QTcpSocket>
#include <QObject>
#include <QTimer>
#include <QSocketNotifier>
#include "consolereader.h"
#include "broadcastsocket.h"

class DummyClient : public QObject
{
    Q_OBJECT
public:
    explicit DummyClient(QObject *parent = 0);
    void run();

protected:
 //   void timerEvent(QTimerEvent*);

signals:
    void incomingMessage(QString);
    void inputFromCommandLine(QString);
    void quit();
    void runConsole();
    void write(QString);
    void closeUdpSocket();
    void openUdpSocket();
    void manualCloseUDP();
    void manualStartUDP();

public slots:
//    void writeToConsole(QString);
    void sendMessage(QString);
    void newDataAvailable();
    void sendFirstMessage();
    void parseInputFromCommandLine(QString text);
    void echo(QString message);

private:
    QString deviceId;
    quint16 serverPort;
    QString serverAddress;
    QString userName;
    QTcpSocket *socket;
    BroadcastSocket *broadcastReceiver;
    ConsoleReader *cReader;
    QThread *consoleThread;
    void connectToServer();
    void closeSocket();
    void Disconnect();
    void startCommand(QString);
    void Quit();
    quint32 qstringToQuint32(QString string);
    QString datagramNeeded;
    bool isEcho;
};

#endif // DUMMYCLIENT_H
