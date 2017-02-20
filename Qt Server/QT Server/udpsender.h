﻿#ifndef UDPSENDER_H
#define UDPSENDER_H

#include <memory>
#include <QDebug>
#include <QObject>
#include <QtNetwork>
#include "Device.h"

class QTimer;
class QUdpSocket;

class UdpSender : public QObject {
    Q_OBJECT

  public:
    UdpSender(std::vector<Device*>& addedDevices, QObject* parent = Q_NULLPTR);

  private slots:
    void StartUdp();
    void StopUdp();
    void SendDatagram();

  private:
    QByteArray datagram;
    QUdpSocket* udpSocket;
    QTimer* timer;
    int messageNo;
    int udpPort;
    std::vector<Device*>& addedDevices;
};

#endif
