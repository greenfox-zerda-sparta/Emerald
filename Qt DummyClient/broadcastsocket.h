#ifndef BROADCASTSOCKET_H
#define BROADCASTSOCKET_H
#include <QObject>
#include <QtNetwork>


class BroadcastSocket : public QObject
{
    Q_OBJECT
public:
    BroadcastSocket(QObject *parent = 0);

signals:
    void newDatagram(QString);

private slots:
     void processPendingDatagrams();
	 void close();
     void startUDP();

 private:
     QUdpSocket *udpSocket;
     bool isRunning;
};

#endif // BROADCASTSOCKET_H
