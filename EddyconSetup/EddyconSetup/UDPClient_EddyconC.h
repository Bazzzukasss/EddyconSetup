#ifndef UDPCLIENT_EDDYCONC_H
#define UDPCLIENT_EDDYCONC_H

#include <QObject>
#include <QString>
#include <QUdpSocket>
#include <UDPClient.h>
#include "TNetDevice.h"

enum HWEddyconServerCmdResultsEnum{
    RES_OK=0xFFFFFFFF,
    RES_ERROR=0xEEEEEEEE,
    RES_UNKNOWN_CMD=0xCCCCCCCC
};

//1-st byte of packet
enum HWEddyconServerCmdEnum{
    CMD_SRV=1,
    CMD_SET,
    CMD_GET,
    CMD_RUN
};
//2-nd byte of packet
enum HWEddyconServerCmdSrvEnum{
    CMD_SRV_KILL=1,
    CMD_SRV_TEST,
    CMD_SRV_REPEAT,
    CMD_SRV_RESTART,
    CMD_SRV_VERSION,
    CMD_SRV_BASH
};
enum HWEddyconServerSetCmdEnum{
    CMD_SET_CHANNEL=1,
    CMD_SET_MIX,
    CMD_SET_MEASCURSOR,
    CMD_SET_MAINSETS,
    CMD_SET_SYSSETS,
    CMD_SET_BALSETS,
    CMD_SET_DATABUFFER
};
enum HWEddyconServerSetDataBuffer{
    CMD_SET_DATABUFFER_SEND_SMP_COUNT=1
};
enum HWEddyconServerSetChannelCmdEnum{
    CMD_SET_CHANNEL_SETS=1,
    CMD_SET_CHANNEL_FILTER,
    CMD_SET_CHANNEL_ASD,
    CMD_SET_CHANNEL_KALIBRCURVE
};
enum HWEddyconServerGetCmdEnum{
    CMD_GET_XYDATA=1,
    CMD_GET_RAWDATA,
    CMD_GET_PARAMS,
    CMD_GET_TESTER,
    CMD_GET_CHANNEL
};
enum HWEddyconServerRunCmdEnum{
    CMD_RUN_PREPARING=1,
    CMD_RUN_START,
    CMD_RUN_STOP,
    CMD_RUN_CREATECURVE,
    CMD_RUN_LOADING,
    CMD_RUN_BALANSING,
    CMD_RUN_CENTERING,
    CMD_RUN_ROTATING,
    CMD_RUN_CLEARING,
    CMD_RUN_ADJUSTING
};
//3-d byte of packet
enum HWEddyconServerGetChannelCmdEnum{
    CMD_GET_CHANNEL_NB_AMP=1,
    CMD_GET_CHANNEL_TRAKT_AMP,
    CMD_GET_CHANNEL_BALANCE_GAIN,
    CMD_GET_CHANNEL_BALANCE_PHASE,
    CMD_GET_CHANNEL_CAPASITOR,
    CMD_GET_CHANNEL_ROTATION,
    CMD_GET_CHANNEL_KU2DELTAX,
    CMD_GET_CHANNEL_KU2DELTAY
};

#define PACK_CMD( byte0 , byte1 , byte2 , byte3 ) ((byte0)|(byte1<<8)|(byte2<<16)|(byte3<<24))

class UDPClient_EddyconC : public UDPClient
{
    Q_OBJECT
public:
    explicit UDPClient_EddyconC(const QString& _ip, int _port,TNetDevice* pDevice,QObject *parent = 0);
    ~UDPClient_EddyconC();
    inline TNetDevice* getNetDevice()           {return pNetDevice;}
    void send_CMD_SRV_TEST()                    {send_CMD(CMD_SRV,CMD_SRV_TEST,0,0);}
    void send_CMD_SRV_KILL()                    {send_CMD(CMD_SRV,CMD_SRV_KILL,0,0);}
    void send_CMD_SRV_REPEAT()                  {send_CMD(CMD_SRV,CMD_SRV_REPEAT,0,0);}
    void send_CMD_SRV_VERSION()                 {send_CMD(CMD_SRV,CMD_SRV_VERSION,0,0);pNetDevice->setInfoReady(true);}
    void send_CMD_SRV_RESTART()                 {send_CMD(CMD_SRV,CMD_SRV_RESTART,0,0);}
    void send_CMD_SRV_BASH()                    {send_CMD(CMD_SRV,CMD_SRV_BASH,0,0,pNetDevice->getBashCmdLineSize(),reinterpret_cast<hwpacket_t*>(pNetDevice->getBashCmdLine()));}
    //RUN CMD
    void send_CMD_RUN_START()                   {send_CMD(CMD_RUN,CMD_RUN_START,0,0);}
    void send_CMD_RUN_STOP()                    {send_CMD(CMD_RUN,CMD_RUN_STOP,0,0);}
    void send_CMD_RUN_LOADING()                 {send_CMD(CMD_RUN,CMD_RUN_LOADING,0,0);}
    void send_CMD_RUN_BALANSING()               {send_CMD(CMD_RUN,CMD_RUN_BALANSING,0,0,false);}
    void send_CMD_RUN_CENTERING()               {send_CMD(CMD_RUN,CMD_RUN_CENTERING,0,0,false);}
    void send_CMD_RUN_ROTATING()                {send_CMD(CMD_RUN,CMD_RUN_ROTATING,0,0,false);}
    void send_CMD_RUN_PREPARING()               {send_CMD(CMD_RUN,CMD_RUN_PREPARING,0,0,false);}
    void send_CMD_RUN_CLEARING()                {send_CMD(CMD_RUN,CMD_RUN_CLEARING,0,0);}
    void send_CMD_RUN_ADJUSTING()               {send_CMD(CMD_RUN,CMD_RUN_ADJUSTING,0,0,false);}
    void send_CMD_RUN_CREATECURVE()             {send_CMD(CMD_RUN,CMD_RUN_CREATECURVE,0,0,false);}
    //void send_CMD_RUN_BUSH()                    {send_CMD(CMD_RUN,CMD_RUN_BASH,0,0,pNetDevice->getBashCmdLineSize(),reinterpret_cast<hwpacket_t*>(pNetDevice->getBashCmdLine()));}
    //GET CMD
    void send_CMD_GET_XYDATA(int _format,int _isGetAllData,int _packet_size=PACKET_DATA_SIZE_B){packet.setHeaderParamValue(0,_format);packet.setHeaderParamValue(1,_isGetAllData);packet.setHeaderParamValue(2,_packet_size);send_CMD(CMD_GET,CMD_GET_XYDATA,0,0,false);}
    void send_CMD_GET_RAWDATA()                 {send_CMD(CMD_GET,CMD_GET_RAWDATA,0,0,false);}
    void send_CMD_GET_PARAMS()                  {send_CMD(CMD_GET,CMD_GET_PARAMS,0,0);}
    void send_CMD_GET_TESTER(int _chan,int _isNoisAmp,int _isZazorCorection)                    {packet.setHeaderParamValue(0,_chan);packet.setHeaderParamValue(1,_isNoisAmp);packet.setHeaderParamValue(2,_isZazorCorection);send_CMD(CMD_GET,CMD_GET_TESTER,0,0,false);}
    void send_CMD_GET_CHANNEL_NB_AMP()          {send_CMD(CMD_GET,CMD_GET_CHANNEL,CMD_GET_CHANNEL_NB_AMP,0);}
    void send_CMD_GET_CHANNEL_TRAKT_AMP()       {send_CMD(CMD_GET,CMD_GET_CHANNEL,CMD_GET_CHANNEL_TRAKT_AMP,0);}
    void send_CMD_GET_CHANNEL_BALANCE_GAIN()    {send_CMD(CMD_GET,CMD_GET_CHANNEL,CMD_GET_CHANNEL_BALANCE_GAIN,0);}
    void send_CMD_GET_CHANNEL_BALANCE_PHASE()   {send_CMD(CMD_GET,CMD_GET_CHANNEL,CMD_GET_CHANNEL_BALANCE_PHASE,0);}
    void send_CMD_GET_CHANNEL_CAPASITOR()       {send_CMD(CMD_GET,CMD_GET_CHANNEL,CMD_GET_CHANNEL_CAPASITOR,0);}
    void send_CMD_GET_CHANNEL_ROTATION()        {send_CMD(CMD_GET,CMD_GET_CHANNEL,CMD_GET_CHANNEL_ROTATION,0);}
    void send_CMD_GET_CHANNEL_KU2DELTAX()       {send_CMD(CMD_GET,CMD_GET_CHANNEL,CMD_GET_CHANNEL_KU2DELTAX,0);}
    void send_CMD_GET_CHANNEL_KU2DELTAY()       {send_CMD(CMD_GET,CMD_GET_CHANNEL,CMD_GET_CHANNEL_KU2DELTAY,0);}

    //SET CMD
    void send_CMD_SET_DATABUFFER_SEND_SMP_COUNT(int _send_smp_count){packet.setHeaderParamValue(0,_send_smp_count);send_CMD(CMD_SET,CMD_SET_DATABUFFER,CMD_SET_DATABUFFER_SEND_SMP_COUNT,0);}
    void send_CMD_SET_MEASCURSOR()                                  {send_CMD(CMD_SET,CMD_SET_MEASCURSOR,0,0,           pNetDevice->getMeasCursorSetsSize(),        reinterpret_cast<hwpacket_t*>(pNetDevice->getMeasCursorSets()));}
    void send_CMD_SET_MAINSETS()                                    {send_CMD(CMD_SET,CMD_SET_MAINSETS,0,0,             pNetDevice->getMainSetsSize(),              reinterpret_cast<hwpacket_t*>(pNetDevice->getMainSets()));}
    void send_CMD_SET_SYSSETS()                                     {send_CMD(CMD_SET,CMD_SET_SYSSETS,0,0,              pNetDevice->getSysSetsSize(),               reinterpret_cast<hwpacket_t*>(pNetDevice->getSysSets()));}
    void send_CMD_SET_BALSETS()                                     {send_CMD(CMD_SET,CMD_SET_BALSETS,0,0,              pNetDevice->getBalanseSetSize(),            reinterpret_cast<hwpacket_t*>(pNetDevice->getBalanseSets()));}
    void send_CMD_SET_CHANNEL_SETS(int _chan,int _isAllParamSet)    {packet.setHeaderParamValue(0,_chan);packet.setHeaderParamValue(1,_isAllParamSet);send_CMD(CMD_SET,CMD_SET_CHANNEL,CMD_SET_CHANNEL_SETS,0,         pNetDevice->getChanSetsSize(),              reinterpret_cast<hwpacket_t*>(pNetDevice->getChanSets(_chan)));}
    void send_CMD_SET_CHANNEL_FILTER(int _chan)                     {packet.setHeaderParamValue(0,_chan);send_CMD(CMD_SET,CMD_SET_CHANNEL,CMD_SET_CHANNEL_FILTER,0,       pNetDevice->getChanFilterSetsSize(),        reinterpret_cast<hwpacket_t*>(pNetDevice->getChanFilterSets(_chan)));}
    void send_CMD_SET_CHANNEL_ASD(int _chan)                        {packet.setHeaderParamValue(0,_chan);send_CMD(CMD_SET,CMD_SET_CHANNEL,CMD_SET_CHANNEL_ASD,0,          pNetDevice->getChanASDSetsSize(),           reinterpret_cast<hwpacket_t*>(pNetDevice->getChanASDSets(_chan)));}
    void send_CMD_SET_CHANNEL_KALIBRCURVE(int _chan)                {packet.setHeaderParamValue(0,_chan);send_CMD(CMD_SET,CMD_SET_CHANNEL,CMD_SET_CHANNEL_KALIBRCURVE,0,  pNetDevice->getChanKalibrCurveSetsSize(),   reinterpret_cast<hwpacket_t*>(pNetDevice->getChanKalibrCurveSets(_chan)));}
    void send_CMD_SET_MIX(int _mix)                                 {packet.setHeaderParamValue(0,_mix); send_CMD(CMD_SET,CMD_SET_MIX,0,0,                  pNetDevice->getMixSetsSize(),               reinterpret_cast<hwpacket_t*>(pNetDevice->getMixSets(_mix)));}
protected:
    virtual void CMD_handler();
    virtual void CMD_SRV_handler();
    virtual void CMD_GET_handler();
    virtual void CMD_GET_CHANNEL_handler();
    virtual void CMD_RUN_handler();
    inline void send_CMD(char _byte0,char _byte1,char _byte2,char _byte3,bool _isWaitForAnswer=true)
    {
        send_CMD(_byte0,_byte1,_byte2,_byte3,0,0,_isWaitForAnswer);
    }
    inline void send_CMD(char _byte0,char _byte1,char _byte2,char _byte3,int _data_size_b,hwpacket_t* _data,bool _isWaitForAnswer=true)
    {
        QMutexLocker locker(&mutex);
        hwpacket_t cmd =PACK_CMD(_byte0,_byte1,_byte2,_byte3);
        QHostAddress* host_ip  = new QHostAddress(ip);
        packet.fill(cmd,0,_data_size_b,_data);
        socket->writeDatagram(reinterpret_cast<char*>(packet.getPacket()),packet.getSendSize(),*host_ip, port);
        socket->waitForBytesWritten(500);
        if(_isWaitForAnswer)
            if(!socket->waitForReadyRead(500))
                qDebug()<<"Wait timeout!:"<<hex<<cmd;
    }

private:
    TNetDevice* pNetDevice;
signals:

public slots:

};

#endif // UDPCLIENT_EDDYCONC_H
