#ifndef BROADCASTSOCKET_H
#define BROADCASTSOCKET_H
#include <QObject>
#include <QtNetwork>


class BroadcastSocket : public QObject
{
    Q_OBJECT
public:
    BroadcastSocket(QString datagramNeeded, QObject *parent = 0);

signals:
    void newDatagram(QString);
    void write(QString);

private slots:
     void processPendingDatagrams();
	 void close();
     void manualClose();
     void startUDP();
     void manualStart();

 private:
     QUdpSocket *udpSocket;
     bool gotFirstMessage;
     void parseDatagram(QString datagram);
     QString datagramNeeded;
     bool isUdpManual;
};

#endif // BROADCASTSOCKET_H
