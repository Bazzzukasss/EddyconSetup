#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>
#include <QString>
#include <QUdpSocket>
#include <QMutex>
#include "HWNetPacket.hpp"
#include "unistd.h"

#define CMD_WAIT_TRY_COUNT      10
#define CMD_WAIT_INTERVAL_US    1000
class UDPClient : public QObject
{
    Q_OBJECT
public:
    explicit UDPClient(const QString& _ip, int _port,QObject *parent = 0);
    ~UDPClient();
    void selfTest();
 protected:
    HWNetPacket packet;

    QUdpSocket *socket;
    int port;
    QString ip;
    QHostAddress host_ip;
    hwpacket_t lastReceivedCmd;
    volatile bool isCmdReceived;
    QMutex mutex;
    virtual void CMD_handler(){};
private:

signals:

public slots:
    void reciveHandler();
    bool reconnect(const QString& _ip, int _port);
    bool reconnect();

};

#endif // UDPCLIENT_H
