#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H



#include <QHostAddress>
#include <QObject>
#include <QTimer>

class QTcpSocket;

class ClientSocket : public QObject
{
    Q_OBJECT
public:
    ClientSocket(QObject *parent = 0);

    ~ClientSocket();
    quint16 serverPort;
    QString serverAddress;
    QString userName;
    void StartTimer();
    void StopTimer();
    void Disconnect();

signals:
    void incomingMessage(QString);

protected slots:
    void connectToServer();
    void newDataAvailable();
    void onSend(QString message);
    void closeSocket();

protected:
    void timerEvent(QTimerEvent*);
private:
    QTcpSocket * socket;
    QByteArray incomingData;
    int timerId;
    bool isFirstMessageSent;
};
#endif // CLIENTSOCKET_H
