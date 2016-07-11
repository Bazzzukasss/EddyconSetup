#include "UDPClient.h"
#include <QMutexLocker>
UDPClient::UDPClient(const QString& _ip, int _port, QObject *parent) :
    QObject(parent)
{
    // create a QUDP socket
    socket = new QUdpSocket(this);
    ip=_ip;
    host_ip.setAddress(_ip);
    port=_port;

    //reconnect(_ip,_port);
}
UDPClient::~UDPClient()
{
    delete socket;
}
bool UDPClient::reconnect(const QString& _ip, int _port)
{
    ip=_ip;
    host_ip.setAddress(_ip);
    port=_port;
    qDebug()<<"reconnect socket to ip:"<<_ip<<endl;
    if(!socket->bind(host_ip, port))
    {
        connect(socket, SIGNAL(readyRead()), this, SLOT(reciveHandler()));
        return true;
    }
    else
        return false;
}
bool UDPClient::reconnect()
{
    qDebug()<<"connect socket to ip:"<<ip<<endl;
    if(!socket->bind(host_ip, port))
    {
        connect(socket, SIGNAL(readyRead()), this, SLOT(reciveHandler()));
        return true;
    }
    else
        return false;
}
void UDPClient::selfTest()
{
    QByteArray Data;
    Data.append("selfTest UDPClient");
    socket->writeDatagram(Data, host_ip, port);
}
/*
void UDPClient::setCmdToWait(hwpacket_t _cmd)
{
    QMutexLocker locker(&mutex);
    CmdToWait=_cmd;
    isCmdWaiting=1;
    //qDebug()<<"Wait cmd setted:"<<hex<<_cmd;
}

void UDPClient::clearCmdToWait(hwpacket_t _cmd)
{
    QMutexLocker locker(&mutex);
    if(_cmd==CmdToWait)
    {
        isCmdWaiting=0;
        //qDebug()<<"Wait cmd cleared:"<<hex<<_cmd;
    }
}

bool UDPClient::waitingCmd(unsigned int _count, unsigned int _interval_us)
{
    unsigned int try_count=0;
    do{
        if(!isCmdWaiting)return true;
        qDebug()<<".";
        if(++try_count>=_count)
            return false;
        usleep(_interval_us*100);
    }
    while(true);
    return false;
}
*/
void UDPClient::reciveHandler()
{
    QHostAddress sender;
    quint16 senderPort;

    packet.setRecivedSize(socket->pendingDatagramSize());
    socket->readDatagram(reinterpret_cast<char*>(packet.getPacket()), packet.getRecivedSize(),&sender, &senderPort);

    //qDebug() << "[UDP Client reciveHandler]";
    //qDebug() << " host: " << sender.toString();
    //qDebug() << " port: " << senderPort;

    if(packet.getRecivedSize()>0)
        CMD_handler();
}
