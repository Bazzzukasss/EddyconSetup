#include <UDPClient_EddyconC.h>
#include "mainwindow.h"
UDPClient_EddyconC::UDPClient_EddyconC(const QString& _ip, int _port,TNetDevice* pDevice, QObject *parent) :
    UDPClient(_ip,_port,parent)
{
    pNetDevice=pDevice;
}

UDPClient_EddyconC::~UDPClient_EddyconC()
{
}

void UDPClient_EddyconC::CMD_RUN_handler()
{
    int level=1,max_param_num=6;
    int chan_count=packet.getHeaderParamValue(0);
    int* pDataI=reinterpret_cast<int*>(packet.getData());
    float* pDataF=reinterpret_cast<float*>(packet.getData());
    //packet.show();
    switch(packet.getCmd(level))
    {
        case CMD_RUN_LOADING:
            pNetDevice->setBoardSmpFreq(packet.getHeaderParamValue(0),packet.getHeaderParamValue(1));
        break;
        case CMD_RUN_BALANSING:
        case CMD_RUN_CENTERING:

        for(int param_num=0;param_num<max_param_num;param_num++)
        {
            for(int chan=0;chan<chan_count;chan++)
            {
                switch(param_num)
                {
                    case 0:
                        pNetDevice->getChanSets(chan)->balanceGain_t=pDataI[chan+chan_count*param_num];
                        break;
                    case 1:
                        pNetDevice->getChanSets(chan)->balancePhase_dg=pDataF[chan+chan_count*param_num];
                        break;
                    case 2:
                        pNetDevice->getChanSets(chan)->Ku2DeltaX=pDataI[chan+chan_count*param_num];
                        qDebug()<<"pDataI[chan+chan_count*param_num]="<<pDataI[chan+chan_count*param_num];
                        break;
                    case 3:
                        pNetDevice->getChanSets(chan)->Ku2DeltaY=pDataI[chan+chan_count*param_num];
                        break;
                    case 4:
                        //trakt amp
                        break;
                    case 5:
                        //nebalance amp
                        break;
                }
            }
        }
            break;
        case CMD_RUN_ADJUSTING:
            for(int chan=0;chan<chan_count;chan++)
                pNetDevice->getChanSets(chan)->capacitor=pDataI[chan];
            break;
        case CMD_RUN_ROTATING:
            for(int chan=0;chan<chan_count;chan++)
                pNetDevice->getChanSets(chan)->rotation=pDataF[chan];
            break;
        default:
            break;
    }
    pNetDevice->setChannelsReady(true);
}
void UDPClient_EddyconC::CMD_GET_handler()
{
    int level=1;
    switch(packet.getCmd(level))
    {
        case CMD_GET_RAWDATA:
            pNetDevice->setRawBuffer(packet.getHeaderParamValue(0),packet.getHeaderParamValue(1),packet.getData());
            break;
        case CMD_GET_TESTER:
            pNetDevice->setTester(packet.getData());
            break;
        case CMD_GET_XYDATA:
            //packet.show();
            pNetDevice->setXYData(packet.getHeaderParamValue(0),packet.getHeaderParamValue(1),packet.getHeaderParamValue(2),packet.getHeaderParamValue(3),packet.getHeaderParamValue(4),packet.getHeaderParamValue(5),packet.getData());
            pNetDevice->setRVTPData(packet.getHeaderParamValue(6),packet.getHeaderParamValue(7));
            break;
        case CMD_GET_CHANNEL:
            CMD_GET_CHANNEL_handler();
            break;
        case CMD_GET_PARAMS:
            pNetDevice->showNetParams(packet.getData());
            break;
    }
}
void UDPClient_EddyconC::CMD_GET_CHANNEL_handler()
{
    int level=2;
    int chan_count=packet.getHeaderParamValue(0);
    int* pDataI=reinterpret_cast<int*>(packet.getData());
    float* pDataF=reinterpret_cast<float*>(packet.getData());
    switch(packet.getCmd(level))
    {
        case CMD_GET_CHANNEL_BALANCE_GAIN:
            for(int chan=0;chan<chan_count;chan++)
                pNetDevice->getChanSets(chan)->balanceGain_t=pDataI[chan];
            break;
        case CMD_GET_CHANNEL_BALANCE_PHASE:
            for(int chan=0;chan<chan_count;chan++)
                pNetDevice->getChanSets(chan)->balancePhase_dg=pDataF[chan];
            break;
        case CMD_GET_CHANNEL_CAPASITOR:
            for(int chan=0;chan<chan_count;chan++)
                pNetDevice->getChanSets(chan)->capacitor=pDataI[chan];
            break;
        case CMD_GET_CHANNEL_NB_AMP:
            //for(int chan=0;chan<chan_count;chan++)
                //pNetDevice->getChanSets(chan)->balanceGain_t=pDataI[chan];
            break;
        case CMD_GET_CHANNEL_TRAKT_AMP:
            //for(int chan=0;chan<chan_count;chan++)
                //pNetDevice->getChanSets(chan)->balanceGain_t=pDataI[chan];
            break;
        case CMD_GET_CHANNEL_ROTATION:
            for(int chan=0;chan<chan_count;chan++)
                pNetDevice->getChanSets(chan)->rotation=pDataF[chan];
            break;
        case CMD_GET_CHANNEL_KU2DELTAX:
            for(int chan=0;chan<chan_count;chan++)
                pNetDevice->getChanSets(chan)->Ku2DeltaX=pDataI[chan];
            break;
        case CMD_GET_CHANNEL_KU2DELTAY:
            for(int chan=0;chan<chan_count;chan++)
                pNetDevice->getChanSets(chan)->Ku2DeltaY=pDataI[chan];
            break;
        default:
            break;
    }
    pNetDevice->setChannelsReady(true);
}
void UDPClient_EddyconC::CMD_SRV_handler()
{
    int level=1;
    switch(packet.getCmd(level))
    {
        case CMD_SRV_VERSION:
            pNetDevice->setVersion(packet.getData());
            break;
    }
}
void UDPClient_EddyconC::CMD_handler()
{
    int level=0;
    if(packet.getRes()==RES_ERROR)
    {
        pNetDevice->setErrorPacketHeader(packet.getHeader());
        //packet.show();
    }
    else
    {
        switch(packet.getCmd(level))
        {
            case CMD_SRV:
                CMD_SRV_handler();
                break;
            case CMD_SET:
                break;
            case CMD_GET:
                CMD_GET_handler();
                break;
            case CMD_RUN:
                CMD_RUN_handler();
                break;
        }
    }
    //packet.show();
}
