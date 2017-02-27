#ifndef DUMMYCLIENT_H
#define DUMMYCLIENT_H

#include <QHostAddress>
#include <QTcpSocket>
#include <QObject>
#include <QTimer>
#include <QSocketNotifier>
#include "consolereader.h"
#include "broadcastsocket.h"
#include "messages.h"
#include <vector>

class DummyClient : public QObject {
    Q_OBJECT
  public:
    explicit DummyClient(QObject* parent = 0);
    void run();

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
    void setConsoleReaderCommandMode(int);

  public slots:
    void sendMessage(QString);
    void newDataAvailable();
    void trackConnectedState();
    void parseInputFromCommandLine(QString text);
    void reactToIncomingMessage(QString message);
    void addDevice(QString newDevDescription = "");
    void removeDevice(QString id = "");
    void setDevice(QString which = "");

  private:
    QString deviceId;
    quint16 serverPort;
    QString serverAddress;
    QString userName;
    QTcpSocket* socket;
    BroadcastSocket* broadcastReceiver;
    ConsoleReader* cReader;
    QThread* consoleThread;
    void connectToServer();
    void closeSocket();
    void Disconnect();
    void startCommand(QString);
    void Quit();
    quint32 qstringToQuint32(QString string);
    QString datagramNeeded;
    Dev me;
    void changeDev();
    Messages messGetter;
    void sendMessage(QByteArray);
    bool isUdpOn;
    bool isDevOn;
    void printHelp();
    void printWhichMessage(QByteArray msg);
    QString getAddDeviceMessage(int index);
    std::vector<QString> addDeviceMessages;
    QString availableDevices;
    QString availableRooms;
    QString availableFloors;
    QString newDeviceDescription;
};

#endif // DUMMYCLIENT_H
