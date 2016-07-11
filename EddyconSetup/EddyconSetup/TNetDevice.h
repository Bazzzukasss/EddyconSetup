#ifndef NETDEVICE_H
#define NETDEVICE_H
#include "HWNetSets.h"
#include "HWNetPacket.hpp"
#include <string.h>
#include <iostream>
#include <QVector>
#include <QString>
#include <QLibrary>
#include "stdint.h"
#include <QProcess>
#include <QStringList>
#include <QFile>
#include <QMutexLocker>


//void __stdcall DLLFilter_calcFilterKoefs(NI_AALBase_FilterPassbandType ftype,double fsmp, double fc, int16_t a_array[], int16_t b_array[], int32_t a_len,int32_t b_len);
typedef __stdcall void (*TDLLFunc)(uint32_t ftype,double fsmp,double fc, int16_t* a_array, int16_t* b_array,int32_t a_len, int32_t b_len);
//typedef __stdcall void (*TDLLFunc)(uint32_t ftype,uint16_t fmode, uint16_t order, double ripple, double attenuation, double fsmp,double fc, int16_t* a_array, int16_t* b_array, uint16_t* i_array,double* d_array, int32_t a_len, int32_t b_len, int32_t i_len, int32_t d_len);
typedef __stdcall void (*TDllFuncTest)(uint16_t a1,uint16_t a2,uint16_t a3,uint16_t a4,uint16_t a5,uint16_t a6,uint16_t* buf,uint16_t len);
enum ViewTypeEnim{VT_XY,VT_MULTY};
enum MultyType{MT_X,MT_Y,MT_A};
class TNetDevice{

public:
    QMutex mutex;
    explicit TNetDevice(int _max_freq_number,int _max_mix_number,int _raw_buf_points_count,int _raw_buf_max_amp,int _data_buf_total_smp,int _sweep_buf_points_count);
    ~TNetDevice();

    int addChannel();
    bool delChannel();
    void calcSpectr(int _len=1024);
    void fillChannelASDSets(int _chan,int _scale,int _type,int _radius,int _bRadius,int _width,int _height,int _alfa,int _beta,int _isSimetric,int _polarity,int _pX0,int _pY0,int _pX1,int _pY1,int _direction);
    void fillChannelFilterSets(int _chan,int _type,int _aver_value,int _dif_value,int _lf_value,int _hf_value,int _iir_lf_value,int _iir_hf_value);
    void fillChannelSets(int _chan,int _frequency_hz,float _voltage_v,float _gain1_db,float _gain2_db,float _h_gain_db,float _v_gain_db,float _rotation,int _capacitor,int _input_type,int _ext_switch,int _balanceGain_t,float _balancePhase_dg,short int _Ku2DeltaX,short int _Ku2DeltaY);
    //get
    inline int getRvtpSmpCountToDraw()
    {
        return rvtp_smp_count;
    }
    inline int getSweepSmpCountToDraw()
    {
        int count;
        if(data_buf_smp_count<sweep_width)
        {
            data_buf_sweep_smp_drawn=0;
            count=data_buf_smp_count;
        }
        else
        {
            data_buf_sweep_smp_drawn=data_buf_smp_count-sweep_width;
            count=sweep_width;
        }
        return count;
    }

    inline int getXYSmpCountToDraw()
    {
        if(data_buf_xy_smp_drawn<=data_buf_smp_count)
            return (data_buf_smp_count-data_buf_xy_smp_drawn);
        else
            return (data_buf_total_smp-data_buf_xy_smp_drawn);
    }
    //inline void incXYSmpDrawnCount(int _drawn_smp)       {data_buf_xy_smp_drawn+=_drawn_smp;}
    //inline void incSweepSmpDrawnCount(int _drawn_smp)    {data_buf_sweep_smp_drawn+=_drawn_smp;}
    inline void incSmpCount()
    {
        data_buf_smp_count++;
        if(data_buf_smp_count>=data_buf_total_smp)
        {
            data_buf_smp_count=0;
            data_buf_xy_smp_drawn=0;
            data_buf_sweep_smp_drawn=0;
        }
    }
    inline bool isNewSmpExist()
    {
        static int old_data_buf_smp_count=0;

        if (abs(data_buf_smp_count-old_data_buf_smp_count)>=sweep_width>>6)
        {
            old_data_buf_smp_count=data_buf_smp_count;
            return true;
        }
        else
            return false;
    }

    inline bool getChannelsReady()                          {return channels_ready;}
    inline HWNetTester* getTester()                         {return &tester;}

    inline QVector<double> getDrawXBuf(int _chan){return drawXBuf[_chan];}
    inline QVector<double> getDrawYBuf(int _chan){return drawYBuf[_chan];}
    inline QVector<double> getDrawARawBuf(int _chan){return drawARawBuf[_chan];}
    inline QVector<double> getDrawRvtpXBuf(){return drawRvtpXBuf;}
    inline QVector<double> getDrawRvtpYBuf(){return drawRvtpYBuf;}
    inline QVector<double> getDrawSweepXBuf(){return drawSweepXBuf;}
    inline QVector<double> getDrawSweepYBuf(){return drawSweepYBuf;}
    inline QVector<double> getDrawSweepARawBuf(){return drawSweepARawBuf;}
    inline QVector<double> getDrawSweepRVTPBuf(){return drawSweepRVTPBuf;}
    inline QVector<double> getDrawSweepOV1Buf(){return drawSweepOV1Buf;}
    inline QVector<double> getDrawSweepOV2Buf(){return drawSweepOV2Buf;}
    inline QVector<double> getDrawSweepASDBuf(){return drawSweepASDBuf;}
    inline QVector<double> getDrawSweepRVTPBufS(){return drawSweepRVTPBufS;}

    inline QVector<double> getRawBuf()                      {return rawBuf;}
    inline QVector<double> getSweepBuf()                    {return sweepBuf;}
    inline QVector<double> getChanXBuf(int _chan)           {return chanXBuf[_chan];}
    inline QVector<double> getChanYBuf(int _chan)           {return chanYBuf[_chan];}
    inline HWDType getSweepChanXBuf(int _chan,int _index)   {return chanXBuf[_chan].at(data_buf_sweep_smp_drawn+_index);}
    inline HWDType getSweepChanYBuf(int _chan,int _index)   {return chanYBuf[_chan].at(data_buf_sweep_smp_drawn+_index);}

    inline HWDType getRvtpChanXBuf(int _chan,int _index)    {if(rvtp_beg_smp+_index<data_buf_smp_count) return chanXBuf[_chan].at(rvtp_beg_smp+_index);else return 0;}
    inline HWDType getRvtpChanYBuf(int _chan,int _index)    {if(rvtp_beg_smp+_index<data_buf_smp_count) return chanYBuf[_chan].at(rvtp_beg_smp+_index);else return 0;}

    inline double getSweepChanARawBuf(int _chan,int _index) {return chanARawBuf[_chan].at(data_buf_sweep_smp_drawn+_index);}
    inline double getSweepRVTPBuf(int _index)               {return RVTPBuf.at(data_buf_sweep_smp_drawn+_index);}
    inline double getSweepASDBuf(int _index)                {return ASDBuf.at(data_buf_sweep_smp_drawn+_index);}
    inline double getSweepOV1Buf(int _index)                {return OV1Buf.at(data_buf_sweep_smp_drawn+_index);}
    inline double getSweepOV2Buf(int _index)                {return OV2Buf.at(data_buf_sweep_smp_drawn+_index);}


    inline int getRawBuffMaxAmp()                   {return raw_buf_max_amp;}
    inline int getRawBuffPointsCount()              {return raw_buf_points_count;}
    inline int getRawBuffReady()                    {return raw_buf_ready;}

    inline HWNetSets*               getSets(){return &sets;}
    inline HWNetChan*               getChanSets(int _chan){return &sets.chan[_chan];}
    inline HWNetChanASD*            getChanASDSets(int _chan){return &sets.chanASD[_chan];}
    inline HWNetChanFilter*         getChanFilterSets(int _chan){return &sets.chanFilter[_chan];}
    inline HWNetChanKalibrCurve*    getChanKalibrCurveSets(int _chan){return &sets.chanKalibrCurve[_chan];}
    inline HWNetBalanseSets*        getBalanseSets(){return &sets.balanseSets;}
    inline HWNetMainSets*           getMainSets(){return &sets.mainSets;}
    inline HWNetSysSets*            getSysSets(){return &sets.sysSets;}
    inline HWNetMeasCursor*         getMeasCursorSets(){return &sets.measCursor;}
    inline HWNetMix*                getMixSets(int _mix){return &sets.mix[_mix];}

    inline int getChanSetsSize()            {return sizeof(HWNetChan);}
    inline int getChanASDSetsSize()         {return sizeof(HWNetChanASD);}
    inline int getChanFilterSetsSize()      {return sizeof(HWNetChanFilter);}
    inline int getChanKalibrCurveSetsSize() {return sizeof(HWNetChanKalibrCurve);}
    inline int getBalanseSetSize()          {return sizeof(HWNetBalanseSets);}
    inline int getMainSetsSize()            {return sizeof(HWNetMainSets);}
    inline int getSysSetsSize()             {return sizeof(HWNetSysSets);}
    inline int getMeasCursorSetsSize()      {return sizeof(HWNetMeasCursor);}
    inline int getMixSetsSize()             {return sizeof(HWNetMix);}
    inline int getCurChannel()              {return sets.sysSets.cur_freq;}
    inline int getChannelCount()            {return sets.sysSets.active_freq_number;}
    inline int getSweepWidth()              {return sweep_width;}
    inline int getXYRange()                 {return xy_range;}
    inline int getMaxMixNumber()            {return max_mix_number;}
    inline int getMaxFreqNumber()           {return max_freq_number;}
    //set
    void setRVTPData(int _smp_count,int _beg_smp);
    void setSweepWidth(int _sweep)          {sweep_width=_sweep;}
    void setXYRange(int _range)             {xy_range=_range;}
    void setSmp(int _freq_num,int _smp_num,int _data_format,char* _data);
    void showSmp(int _freq_num,int _smp_num,int _data_format);
    void setXYData(int _freq_num,int _smp_count,int _smp_size_b,int _data_format,int _board_smp_count,int _board_send_smp_count,void* _data);
    inline void setChannelsReady(bool _ready)       {channels_ready=_ready;}
    void setCurChannel(int _chan);
    inline void setTester(void* _data)              {tester=*(reinterpret_cast<HWNetTester*>(_data));tester_ready=true;}
    inline bool getTesterReady()                    {return tester_ready;}
    inline void setTesterReady(bool _ready)         {tester_ready=_ready;}
    inline void setRawBuffReady(int _ready)         {raw_buf_ready=_ready;}
    inline bool getInfoReady()                      {return info_ready;}
    inline void setInfoReady(bool _ready)           {info_ready=_ready;}
    inline void setSignalsReady(bool _ready)        {signals_ready=_ready;}
    inline bool getSignalsReady()                   {return signals_ready;}
    void setRawBuffer(int _ready,int _points_count,void* _data);
    void setErrorPacketHeader(HWPacketHeader* _pHeader){memcpy(&errorPacketHeader,_pHeader,PACKET_HEADER_SIZE_B);}
    void clearDataBuffers()
    {
        data_buf_xy_smp_drawn=0;
        data_buf_sweep_smp_drawn=0;
        data_buf_smp_count=0;
    };
    int getSmpCount(){return data_buf_smp_count;}
    //int getSweepSmpDrawn(){return data_buf_sweep_smp_drawn;}
    int getXYSmpDrawn(){return data_buf_xy_smp_drawn;}
    int getRvtpSmpCount(){return rvtp_smp_count;}
    int getRvtpBegSmp(){return rvtp_beg_smp;}
    int getRvtpEndSmp(){return rvtp_end_smp;}
    void setBoardIP(const QString &_ip)
    {
        board_ip=_ip;
    }

    QString getInfoText()
    {
        infoString.sprintf("<div><b>BOARD IP:</b> %s</div><div><b>BOARD VERSION:</b> %s</div> <div><b>smp count:</b> %d (%d)</div><div><b>latency:</b> %d</div><div><b>F smp:</b> %d Hz (%d Hz)</div><b>smp size:</b> %d bytes</div><div><b>smp count per packet:</b> %d</div><div><b>rvtp smp count:</b> %d</div><div><b>rvtp beg smp:</b> %d</div><div><b>rvtp end smp:</b> %d</div>",board_ip.toStdString().c_str(),version,data_buf_smp_count,board_smp_count,board_latency,board_smp_freq,board_smp_freq_max,board_smp_size_b,board_smp_count_per_packet,rvtp_smp_count,rvtp_beg_smp,rvtp_end_smp);
        //infoString.sprintf("<div><b>BOARD VERSION:</b> %s</div> <div><b>smp count:</b> %d (%d)</div><div><b>latency:</b> %d</div><div><b>smp count per packet:</b> %d</div><div><b>rvtp smp count:</b> %d</div><div><b>rvtp beg smp:</b> %d</div><div><b>rvtp end smp:</b> %d</div>",version,data_buf_smp_count,board_smp_count,board_latency,board_smp_count_per_packet,rvtp_smp_count,rvtp_beg_smp,rvtp_end_smp);
        return infoString;
    }
    QString getTesterString()
    {
        testerString.sprintf("%2d) <font color=\"#CC0000\"> AF=%5.2fV / %6d </font><font color=\"#666666\">(an=%5d as=%5d ksn=%5.2fdB)</font>  <font color=\"#CC0000\">ARaw=%5.2fV / %5d / %3d%%</font> <font color=\"#666666\">(t=%3d%% nb=%3d%%)</font> <font color=\"#CC0000\">Ph=%7.2fdg  D=%5.2fmm</font>   <font color=\"#009900\">RPM=%4d</font> <font color=\"#000099\">Path=%5d</font> ",tester.CurChan,tester.AmpF,tester.AmpI,tester.ANois,tester.ASig,tester.KSN,tester.AmpFRaw,tester.AmpIRaw,tester.AmpRaw_prs,tester.TraktAmp,tester.NebalanseAmp,tester.Phase,tester.DefVal,tester.RPM,tester.Path);
        return testerString;
    }
    void setVersion(void* _data)
    {
        strcpy(version,(reinterpret_cast<char*>(_data)));
    }
    char* getBashCmdLine()
    {
        //bashCmdLine.sprintf("ls");
        return (char*)bashCmdLine.toUtf8().constData();
    }
    int getBashCmdLineSize()
    {
        return bashCmdLine.size();
    }
    void setBashCmdLine(const QString &_str)
    {
        bashCmdLine=_str;
    }
    void setBoardSmpFreq(int _smp_freq,int _smp_freq_max)
    {
        board_smp_freq =_smp_freq;
        board_smp_freq_max = _smp_freq_max;
    }

    void calcChannelFilterKoefs(HWNetChanFilter* pFilter)
    {
        genFilterKoefs(pFilter->type,sets.mainSets.max_Fsmp_hz,pFilter->iir_lfValue,pFilter->iir_hfValue,&pFilter->koefs_buf[0][0],&pFilter->koefs_buf[1][0]);
    }

    void calcChannels()
    {
        for(int chan=0;chan<sets.sysSets.active_freq_number;chan++)
            calcChannelFilterKoefs(&sets.chanFilter[chan]);
    };
    int changeViewType()
    {
        if(++viewType>=2)
            viewType=0;
        return viewType;
    }
    int changeMultyType()
    {
        if(++multyType>=3)
            multyType=0;
        return multyType;
    }


    void setViewType(int _view_type){viewType=_view_type;}
    void setMultyType(int _multy_type){multyType=_multy_type;}
    int getViewType(){return viewType;}
    int getMultyType(){return multyType;}
    bool prepareDrawData();
    void showNetParams(hwpacket_t* pData);
private:
    void initDataBuffers(int _max_freq_number,int _raw_buf_points_count,int _data_buf_total_smp,int _sweep_buf_points_count);
    int viewType;
    int multyType;

    HWPacketHeader errorPacketHeader;
    HWNetSets sets;
    HWNetTester tester;
    bool signals_ready;
    bool info_ready;
    bool channels_ready;
    bool tester_ready;
    int raw_buf_points_count;
    int raw_buf_max_amp;
    int raw_buf_ready;
    int max_freq_number;
    int max_mix_number;
    int data_buf_total_smp;
    int board_smp_count;
    int board_send_smp_count;
    int board_latency;
    int board_smp_count_per_packet;
    int board_smp_size_b;
    int board_smp_freq;
    int board_smp_freq_max;
    int data_buf_smp_count;
    int data_buf_xy_smp_drawn;
    int data_buf_sweep_smp_drawn;
    int elem_size_array[32];
    int sweep_width;
    int xy_range;
    int rvtp_smp_count;
    int rvtp_beg_smp;
    int rvtp_end_smp;
    char version[32];


    QVector<double>     rawBuf;
    QVector<double>     sweepBuf;
    QVector<double>*    chanXBuf;
    QVector<double>*    chanYBuf;
    QVector<double>*    chanABuf;
    QVector<double>*    chanARawBuf;
    QVector<double>     ASDBuf;
    QVector<double>     OV1Buf;
    QVector<double>     OV2Buf;
    QVector<double>     RVTPBuf;
    QVector<double>     DPBuf;

    QVector<double>* drawXBuf;
    QVector<double>* drawYBuf;
    QVector<double>* drawARawBuf;
    QVector<double> drawRvtpXBuf;
    QVector<double> drawRvtpYBuf;
    QVector<double> drawSweepXBuf;
    QVector<double> drawSweepYBuf;
    QVector<double> drawSweepARawBuf;
    QVector<double> drawSweepRVTPBuf;
    QVector<double> drawSweepRVTPBufS;
    QVector<double> drawSweepOV1Buf;
    QVector<double> drawSweepOV2Buf;
    QVector<double> drawSweepASDBuf;

    QString             infoString;
    QString             testerString;
    QString             bashCmdLine;
    QString             board_ip;
    void initSets();
    void BPF(register float *x, register float *y, register int N, register int I);

    QLibrary* TestDLL;
    TDllFuncTest  TestDLL_func;

    QLibrary* IIRFilterDLL;
    TDLLFunc IIRFilterDLL_calcFilterKoefs;

    void gen_koefs(int ftype,int fsmp,int fc,short int* buf);
    void genFilterKoefs(int ftype,int fsmp,int lf,int hf,short int* buf1,short int* buf2);
    void testDLL();
    void testFile();
    void getKoefFromFile(const QString& filename, unsigned int* _k_buf);

};

#endif // NETDEVICE_H
