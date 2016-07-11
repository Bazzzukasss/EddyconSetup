#include "TNetDevice.h"
#include "math.h"
#include <QMessageBox>
#include <QDebug>
TNetDevice::TNetDevice(int _max_freq_number,int _max_mix_number,int _raw_buf_points_count,int _raw_buf_max_amp,int _data_buf_total_smp,int _sweep_buf_points_count)
{
    raw_buf_max_amp=_raw_buf_max_amp;
    max_freq_number=_max_freq_number;
    max_mix_number=_max_mix_number;
    data_buf_total_smp=_data_buf_total_smp;

    initDataBuffers(_max_freq_number,_raw_buf_points_count,_data_buf_total_smp,_sweep_buf_points_count);

    elem_size_array[SDF_INDEX_X]=sizeof(HWDType);
    elem_size_array[SDF_INDEX_Y]=sizeof(HWDType);
    elem_size_array[SDF_INDEX_A]=sizeof(HWDType);
    elem_size_array[SDF_INDEX_ARAW]=sizeof(HWDType);
    elem_size_array[SDF_INDEX_OV1]=4;
    elem_size_array[SDF_INDEX_OV2]=4;
    elem_size_array[SDF_INDEX_RVTP]=1;
    elem_size_array[SDF_INDEX_DP]=4;
    elem_size_array[SDF_INDEX_ASD]=4;

    raw_buf_points_count=0;
    data_buf_smp_count=0;
    data_buf_xy_smp_drawn=0;
    data_buf_sweep_smp_drawn=0;
    initSets();
    board_smp_count=0;
    data_buf_smp_count=0;
    data_buf_sweep_smp_drawn=0;
    data_buf_xy_smp_drawn=0;
    rvtp_smp_count=0;
    rvtp_beg_smp=0;
    rvtp_end_smp=0;
    strcpy(version,"");
/*
    QLibrary* IIRFilterDLL=new QLibrary("dll/EddyconD");
    if (!IIRFilterDLL->load())
        QMessageBox::information(0,"Error","Error EddyconD loading!");
    IIRFilterDLL_calcFilterKoefs=(TDLLFunc)IIRFilterDLL->resolve("DLLFilter_calcFilterKoefs");
    if(!IIRFilterDLL_calcFilterKoefs)
        QMessageBox::information(0,"Error","Error IIRFilterDLL_calcFilterKoefs() resolving!");

    QLibrary* TestDLL=new QLibrary("dll/Test");
    if(!TestDLL->load())
        QMessageBox::information(0,"Error","Error testDLL loading!");
    TestDLL_func=(TDllFuncTest)TestDLL->resolve("Test_DLLSubVI");
    if(!TestDLL_func)
        QMessageBox::information(0,"Error","Error function resolving!");

    testDLL();
*/
    //testFile();
}
void TNetDevice::initDataBuffers(int _max_freq_number,int _raw_buf_points_count,int _data_buf_total_smp,int _sweep_buf_points_count)
{
    sweepBuf.resize(_sweep_buf_points_count);
    for(int i=0;i<_sweep_buf_points_count;i++)
            sweepBuf[i]=i;
    rawBuf.resize(_raw_buf_points_count);

    drawXBuf=new QVector<double>[_max_freq_number];
    drawYBuf=new QVector<double>[_max_freq_number];
    drawARawBuf=new QVector<double>[_max_freq_number];

    chanXBuf=new QVector<double>[_max_freq_number];
    chanYBuf=new QVector<double>[_max_freq_number];
    chanABuf=new QVector<double>[_max_freq_number];
    chanARawBuf=new QVector<double>[_max_freq_number];
    for(int i=0;i<_max_freq_number;i++)
    {
        chanXBuf[i].resize(_data_buf_total_smp);
        chanYBuf[i].resize(_data_buf_total_smp);
        chanABuf[i].resize(_data_buf_total_smp);
        chanARawBuf[i].resize(_data_buf_total_smp);
    }
    ASDBuf.resize(_data_buf_total_smp);
    DPBuf.resize(_data_buf_total_smp);
    OV1Buf.resize(_data_buf_total_smp);
    OV2Buf.resize(_data_buf_total_smp);
    RVTPBuf.resize(_data_buf_total_smp);
}

void TNetDevice::getKoefFromFile(const QString& filename, unsigned int* _k_buf)
{
    QFile file;
    file.setFileName(filename);
    file.open(QIODevice::ReadOnly);
    if(file.isOpen())
    {
        file.read((char*)_k_buf,7*4);
        file.close();
        cout<<"KOEFS:";
        for(int i=0;i<7;i++)
            cout<<_k_buf[i]<<" ";
        cout<<endl;
    }
}

void TNetDevice::testFile()
{
    QProcess vec;
    QStringList args;
    unsigned int k_buf[7];
    cout<<"TestFile:"<<endl;
    args<<"0"<<"1000"<<"100";
    //vec.start("\IIR_to_EDD",args);
    vec.start("\test_arg",args);
    vec.waitForFinished(-1);
    getKoefFromFile("C:\\Koeffs.bin",k_buf);
}

void TNetDevice::testDLL()
{
#pragma pack(push)
#pragma pack(1)

    short int           a_array[6];
    short int           b_array[6];
    //unsigned short int  i_array[6];
    //double              d_array[6];
    unsigned short int  buf[6];

    for(int i=0;i<6;i++)
    {
        a_array[i]=10;
        b_array[i]=10;
        //i_array[i]=10;
        //d_array[i]=10;
    }


    TestDLL_func(10,20,30,40,50,60,buf,6);
    cout<<"testDLL_func:[";
    for(int i=0;i<6;i++)
        cout<<buf[i];
    cout<<"]"<<endl;

    IIRFilterDLL_calcFilterKoefs(0,1000,200,a_array,b_array,3,3);
    cout<<"IIRFilterDLL_calcFilterKoefs:"<<endl;
    cout<<"a_array:"<<hex<<a_array[0]<<" "<<a_array[1]<<" "<<a_array[2]<<endl;
    cout<<"b_array:"<<hex<<b_array[0]<<" "<<b_array[1]<<" "<<b_array[2]<<endl;
#pragma pack(pop)

}
TNetDevice::~TNetDevice()
{
    delete[] sets.chan;
    delete[] sets.chanASD;
    delete[] sets.chanFilter;
    delete[] sets.chanKalibrCurve;
    delete[] sets.mix;
    delete[] chanXBuf;
    delete[] chanYBuf;
    delete[] chanABuf;
    delete[] drawXBuf;
    delete[] drawYBuf;
    delete[] drawARawBuf;
    delete IIRFilterDLL;
}
void TNetDevice::genFilterKoefs(int ftype,int fsmp,int lf,int hf,short int* buf1,short int* buf2)
{
#define IIR_LP_FILTER   13
#define IIR_HP_FILTER   14
#define IIR_BP_FILTER   15

    switch(ftype)
    {
        case IIR_LP_FILTER:
            gen_koefs(0,fsmp,lf,buf1);
            break;
        case IIR_HP_FILTER:
            gen_koefs(1,fsmp,hf,buf2);
            break;
        case IIR_BP_FILTER:
            gen_koefs(0,fsmp,lf,buf1);
            gen_koefs(1,fsmp,hf,buf2);
            break;
    }
}



void TNetDevice::gen_koefs(int ftype,int fsmp,int fc,short int* buf)
{
#define FILTER_GENERATION_ENABLE
#ifdef FILTER_GENERATION_ENABLE
#define DLL_FILTER_GENERATION
    #ifdef DLL_FILTER_GENERATION
        short int   a_array[3];
        short int   b_array[3];


        IIRFilterDLL_calcFilterKoefs(ftype,fsmp,fc,a_array,b_array,3,3);
        buf[0]=a_array[0];
        buf[1]=a_array[1];
        buf[2]=a_array[2];
        buf[3]=b_array[1];
        buf[4]=b_array[2];


    #else

        QProcess vec;
        QStringList args;
        unsigned int k_buf[7];
        args<<QString::number(ftype)<<QString::number(fsmp)<<QString::number(fc);

        //vec.start("\IIR_to_EDD",args);
        vec.start("\test_arg",args);
        vec.waitForFinished(-1);
        getKoefFromFile("C:\\Koeffs.bin",k_buf);

        buf[0]=k_buf[1];
        buf[1]=k_buf[2];
        buf[2]=k_buf[3];

        buf[3]=k_buf[5];
        buf[4]=k_buf[6];
    #endif
    cout<<"Filter type:"<<dec<<ftype<<" fsmp:"<<fsmp<<" fc:"<<fc<<" koefs: "<<hex<<buf[0]<<" "<<buf[1]<<" "<<buf[2]<<" "<<buf[3]<<" "<<buf[4]<<endl;
#endif
}

void TNetDevice::calcSpectr(int _len)
{
    float x[_len],y[_len],X,Y;
    if(_len<=data_buf_smp_count)
    {
        for(int i=0;i<_len;i++)
        {
            X=(float)chanXBuf[sets.sysSets.cur_freq][data_buf_smp_count-_len+i];
            Y=(float)chanYBuf[sets.sysSets.cur_freq][data_buf_smp_count-_len+i];
            y[i]=sqrt(X*X+Y*Y);
            x[i]=0;
        }
        BPF(x,y,_len,1);
        for(int i=0;i<_len;i++)
            rawBuf[i]=sqrt(x[i]*x[i]+y[i]*y[i]);
        raw_buf_points_count=_len/2;
        raw_buf_ready=true;
    }
}

//register float *x,*y; /*x,y-входные массивы данных*/
//register int N,I; /*размерностью I=1 -БПФ I=-1 -ОБПФ*/
void TNetDevice::BPF(register float *x, register float *y, register int N, register int I) /*Процедура БПФ*/
{
    register float c,s,t1,t2,t3,t4,u1,u2,u3;
    register int i,j,p,l,L,M,M1,K;
    L=N;
    M=N/2;
    M1=N-1;
    while(L>=2){
        l=L/2; u1=1.; u2=0.; t1=M_PI/(float)l;
        c=cos(t1); s=(-1)*I*sin(t1);
        for(j=0; j<l;j++){
            for(i=j;i<N;i+=L){
                p=i+l;
                t1=*(x+i) + *(x+p);         //*(x+i)  == x[i]
                t2=*(y+i) + *(y+p);
                t3=*(x+i) - *(x+p);
                t4=*(y+i) - *(y+p);
                *(x+p)=t3 * u1 - t4*u2;
                *(y+p)=t4 * u1 + t3*u2;
                *(x+i)=t1; *(y + i)=t2;
            }
            u3=u1*c-u2*s;
            u2=u2*c+u1*s; u1=u3;
        }
        L/=2;
    }
    j=0;
    for(i=0;i<M1;i++){
        if(i>j){
            t1=*(x+j); t2=*(y+j);
            *(x+j)=*(x+i); *(y+j)=*(y+i);
            *(x+i)=t1; *(y+i)=t2;
        }
        K=M;
        while(j >=K){
            j-=K;K/=2;
        }
        j+=K;
        //printf("i=%d\n",i);
    }
}
void TNetDevice::showSmp(int _freq_num,int _smp_num,int _data_format)
{
    for(int ch=0;ch<_freq_num;ch++)
    {
        if(_data_format&SDF_X)      {cout<<chanXBuf[ch][_smp_num]<<endl;}
        if(_data_format&SDF_Y)      {cout<<chanYBuf[ch][_smp_num]<<endl;}
        if(_data_format&SDF_A)      {cout<<chanABuf[ch][_smp_num]<<endl;}
        if(_data_format&SDF_ARAW)   {cout<<chanARawBuf[ch][_smp_num]<<endl;}
    }
    if(_data_format&SDF_OV1)        {cout<<OV1Buf[_smp_num]<<endl;}
    if(_data_format&SDF_OV1)        {cout<<OV2Buf[_smp_num]<<endl;}
    if(_data_format&SDF_RVTP)       {cout<<RVTPBuf[_smp_num]<<endl;}
    if(_data_format&SDF_DP)         {cout<<DPBuf[_smp_num]<<endl;}
    if(_data_format&SDF_ASD)        {cout<<ASDBuf[_smp_num]<<endl;}
}

void TNetDevice::setSmp(int _freq_num,int _smp_num,int _data_format,char* _data)
{
    char* pData=_data;
    int e=0;
    if(_freq_num==0)
        cout<<"_freq_num="<<_freq_num<<" _data_format="<<_data_format<<" _smp_num="<<_smp_num<<endl;
    for(int ch=0;ch<_freq_num;ch++)
    {
        e=0;

        if(_data_format&SDF_X)      {chanXBuf[ch][_smp_num]=    *(reinterpret_cast<HWDType*>(pData));   pData+=elem_size_array[e++];}
        if(_data_format&SDF_Y)      {chanYBuf[ch][_smp_num]=    *(reinterpret_cast<HWDType*>(pData));   pData+=elem_size_array[e++];}
        if(_data_format&SDF_A)      {chanABuf[ch][_smp_num]=    *(reinterpret_cast<HWDType*>(pData));   pData+=elem_size_array[e++];}
        if(_data_format&SDF_ARAW)   {chanARawBuf[ch][_smp_num]= *(reinterpret_cast<unsigned short int*>(pData));   pData+=elem_size_array[e++];}
    }
    if(_data_format&SDF_OV1)        {OV1Buf[_smp_num]=          *(reinterpret_cast<int*>(pData));       pData+=elem_size_array[e++];}
    if(_data_format&SDF_OV1)        {OV2Buf[_smp_num]=          *(reinterpret_cast<int*>(pData));       pData+=elem_size_array[e++];}
    if(_data_format&SDF_RVTP)       {RVTPBuf[_smp_num]=         *(reinterpret_cast<char*>(pData));      pData+=elem_size_array[e++];}
    if(_data_format&SDF_DP)         {DPBuf[_smp_num]=           *(reinterpret_cast<int*>(pData));       pData+=elem_size_array[e++];}
    if(_data_format&SDF_ASD)        {ASDBuf[_smp_num]=          *(reinterpret_cast<int*>(pData));       pData+=elem_size_array[e++];}
}
void TNetDevice::setRVTPData(int _smp_count,int _beg_smp)
{
    rvtp_smp_count  = _smp_count;
    rvtp_beg_smp    = _beg_smp;
    rvtp_end_smp    = _beg_smp+_smp_count-1;
    //cout<<rvtp_smp_count<<" "<<rvtp_beg_smp<<" "<<rvtp_end_smp<<endl;
}
void TNetDevice::showNetParams(hwpacket_t* pData)
{
    HWNetParams* pParams=reinterpret_cast<HWNetParams*>(pData);
    qDebug()<<"Params:";
    qDebug()<<"\tID:"<<pParams->id;
    qDebug()<<"\tFsmp:"<<pParams->fsmp;
    qDebug()<<"\tDP:"<<pParams->dp_value;
    qDebug()<<"\tDP ROUNDS"<<pParams->dp_rounds;
}

void TNetDevice::setXYData(int _freq_num,int _smp_count_per_packet,int _smp_size_b,int _data_format,int _board_smp_count,int _board_send_smp_count,void* _data)
{
    char* pData=reinterpret_cast<char*>(_data);
    for(int i=0;i<_smp_count_per_packet;i++)
    {
        setSmp(_freq_num,data_buf_smp_count,_data_format,pData);
        //showSmp(_freq_num,data_buf_smp_count,_data_format);
        pData+=_smp_size_b;
        incSmpCount();
    }

    board_smp_count=_board_smp_count;
    board_send_smp_count=_board_send_smp_count;
    board_latency=board_smp_count-board_send_smp_count;
    board_smp_count_per_packet=_smp_count_per_packet;
    board_smp_size_b=_smp_size_b;
    //cout<<"board_smp_count_per_packet:"<<board_smp_count_per_packet<<" board_smp_count"<<_board_smp_count<<" board_send_smp_count:"<<board_send_smp_count<<endl;
}
void TNetDevice::setRawBuffer(int _ready,int _points_count,void* _data)
{
    short int* pData=reinterpret_cast<short int*>(_data);
    raw_buf_ready=_ready;
    if(_ready)
    {
        raw_buf_points_count=_points_count;
        rawBuf.resize(_points_count);
        for(int i=0;i<_points_count;i++)
            rawBuf[i]=pData[i];
    }
    else
        raw_buf_points_count=0;
}
void TNetDevice::initSets(){
        int ch;
        HWNetChan   chanDef;
        HWNetChanASD    chanASDDef;
        HWNetChanFilter chanFilterDef;
        HWNetChanKalibrCurve chanKalibrCurveDef;
        HWNetMix mixDef;

        sets.chan=new HWNetChan[max_freq_number];
        sets.chanASD=new HWNetChanASD[max_freq_number];
        sets.chanFilter=new HWNetChanFilter[max_freq_number];
        sets.chanKalibrCurve=new HWNetChanKalibrCurve[max_freq_number];
        sets.mix=new HWNetMix[max_mix_number];

        memset(&chanASDDef,0,sizeof(HWNetChanASD));
        memset(&chanFilterDef,0,sizeof(HWNetChanFilter));
        memset(&chanKalibrCurveDef,0,sizeof(HWNetChanKalibrCurve));
        memset(&chanDef,0,sizeof(HWNetChan));

        for(int k=0;k<IIR_FILTER_LEN;k++)
        {
            chanFilterDef.koefs_buf[0][k]=k;
            chanFilterDef.koefs_buf[1][k]=k;
        }
        chanASDDef.scale=1;
        chanDef.frequency_hz=95000;
        chanDef.voltage_v=4;
        chanDef.gain1_db=6;
        chanDef.gain2_db=9;
        chanDef.input_type=1;
        mixDef.chan=-1;
        for(ch=0;ch<max_freq_number;ch++)
        {
            sets.chan[ch]=chanDef;
            sets.chanASD[ch]=chanASDDef;
            sets.chanFilter[ch]=chanFilterDef;
            sets.chanKalibrCurve[ch]=chanKalibrCurveDef;
        }

        for(ch=0;ch<max_mix_number;ch++)
            sets.mix[ch]=mixDef;

        sets.mainSets.max_Fsmp_hz=1000;
        sets.mainSets.is_double_freq_mode=1;
        sets.mainSets.sync_type=0;
        sets.mainSets.rvtp_speed_index=0;
        sets.mainSets.measOnMm=1;
        sets.mainSets.pulse_on_meas=10;
        sets.mainSets.rvtpAutoDetectMode=0;
        sets.mainSets.rvtp_rounds_count=1;
        sets.mainSets.rvtp_speed_index=0;
        sets.mainSets.testMode=9;
        sets.mainSets.input_switch=1;
        sets.mainSets.ext_switch_type=0;

        sets.sysSets.meas_dly_t=2500;
        sets.sysSets.meas_len_t=1024;
        sets.sysSets.adc_meas_dly_t=32;
        sets.sysSets.corelation_scale=23;
        sets.sysSets.corelation_deep=1024;
        sets.sysSets.meas_period_num=1;
        sets.sysSets.active_freq_number=1;
        sets.sysSets.cur_freq=0;
}


bool TNetDevice::prepareDrawData()
{
    static int old_smp_to_draw=0;
    static int old_chan_count=0;
    double t,w,center_delta_x,center_delta_y,center_delta;
    QMutexLocker locker(&mutex);
    int cur_freq=getCurChannel();
    int chan_count=getChannelCount();
    int smp_to_draw=getSweepSmpCountToDraw();
    int rvtp_smp_to_draw=getRvtpSmpCountToDraw();
    w=getXYRange()*2;
    t=w/(chan_count+1);
    if(!isNewSmpExist())
        return false;
    if(smp_to_draw>0)
    {
        //client->getNetDevice()->calcSpectr();
        //cout<<"sweep_smp_to_draw="<<smp_to_draw<<endl;
        setInfoReady(true);
        if((smp_to_draw!=old_smp_to_draw)||(old_chan_count!=chan_count))
        {
            old_chan_count=chan_count;
            old_smp_to_draw=smp_to_draw;
            for(int chan=0;chan<chan_count;chan++)
            {
                drawXBuf[chan].resize(smp_to_draw);
                drawYBuf[chan].resize(smp_to_draw);
                drawARawBuf[chan].resize(smp_to_draw);
            }
            if(rvtp_smp_to_draw)
            {
                drawRvtpXBuf.resize(rvtp_smp_to_draw);
                drawRvtpYBuf.resize(rvtp_smp_to_draw);
            }
            drawSweepXBuf.resize(smp_to_draw);
            drawSweepYBuf.resize(smp_to_draw);
            drawSweepARawBuf.resize(smp_to_draw);
            drawSweepRVTPBuf.resize(smp_to_draw);
            drawSweepRVTPBufS.resize(smp_to_draw);
            drawSweepOV1Buf.resize(smp_to_draw);
            drawSweepOV2Buf.resize(smp_to_draw);
            drawSweepASDBuf.resize(smp_to_draw);
        }

        for(int i=0;i<smp_to_draw;i++)
        {
            for(int chan=0;chan<chan_count;chan++)
            {
                center_delta=(chan-chan_count/2)*t;
                if(chan_count%2==0)
                {
                    center_delta+=t/2;
                }
                if(viewType==VT_XY)
                {
                    center_delta_x=center_delta;
                    center_delta_y=0;
                }
                else
                {
                    center_delta_x=center_delta_y=center_delta;
                }
                drawXBuf[chan][i]=getSweepChanXBuf(chan,i)+center_delta_x;
                drawYBuf[chan][i]=getSweepChanYBuf(chan,i)+center_delta_y;
                drawARawBuf[chan][i]=getSweepChanARawBuf(chan,i)+center_delta_y;
            }
            drawSweepXBuf[i]=getSweepChanXBuf(cur_freq,i);//+Cntr;
            drawSweepYBuf[i]=getSweepChanYBuf(cur_freq,i);//-Cntr;
            drawSweepARawBuf[i]=getSweepChanARawBuf(cur_freq,i);
            drawSweepRVTPBufS[i]=getSweepRVTPBuf(i)*(1<<17)-(1<<16);
            drawSweepRVTPBuf[i]=getSweepRVTPBuf(i)+0.1;
            drawSweepOV1Buf[i]=getSweepOV1Buf(i)+1.1;
            drawSweepOV2Buf[i]=getSweepOV2Buf(i)+2.1;
            drawSweepASDBuf[i]=getSweepASDBuf(i)+3.1;
        }
        for(int j=0;j<rvtp_smp_to_draw;j++)
        {
            drawRvtpXBuf[j]=getRvtpChanXBuf(cur_freq,j);
            drawRvtpYBuf[j]=getRvtpChanYBuf(cur_freq,j);
        }
        setSignalsReady(true);
        return true;
    }
    return false;
}

int TNetDevice::addChannel()
{
    if(sets.sysSets.active_freq_number<max_freq_number)
    {
        sets.sysSets.active_freq_number++;
        return sets.sysSets.active_freq_number-1;
    }
    return -1;
}

bool TNetDevice::delChannel()
{
    if(sets.sysSets.active_freq_number>0)
    {
        for(int i=sets.sysSets.cur_freq;i<sets.sysSets.active_freq_number;i++)
        {
            sets.chan[i]=sets.chan[i+1];
            sets.chanASD[i]=sets.chanASD[i+1];
            sets.chanFilter[i]=sets.chanFilter[i+1];
            sets.chanKalibrCurve[i]=sets.chanKalibrCurve[i+1];
        }
        sets.sysSets.active_freq_number--;
        if(sets.sysSets.cur_freq>=sets.sysSets.active_freq_number)
            sets.sysSets.cur_freq=sets.sysSets.active_freq_number-1;
        return true;
    }
    return false;
}

void TNetDevice::setCurChannel(int _chan)
{
    if((_chan>=0)&&(_chan<sets.sysSets.active_freq_number))
            sets.sysSets.cur_freq=_chan;
}

void TNetDevice::fillChannelASDSets(int _chan,int _scale,int _type,int _radius,int _bRadius,int _width,int _height,int _alfa,int _beta,int _isSimetric,int _polarity,int _pX0,int _pY0,int _pX1,int _pY1,int _direction)
{
    HWNetChanASD* _pASD=&sets.chanASD[_chan];
    _pASD->scale=_scale;
    _pASD->Frame.Alfa=_alfa;
    _pASD->Frame.Beta=_beta;
    _pASD->Frame.BRadius=_bRadius;
    _pASD->Frame.Direction=_direction;
    _pASD->Frame.Height=_height;
    _pASD->Frame.isSimetric=_isSimetric;
    _pASD->Frame.PointsCount=FRAME_POINTS_NUMBER;
    _pASD->Frame.Polarity=_polarity;
    _pASD->Frame.Radius=_radius;
    _pASD->Frame.Type=_type;
    _pASD->Frame.Width=_width;
    _pASD->Frame.pX[0]=_pX0;
    _pASD->Frame.pX[1]=_pX1;
    _pASD->Frame.pY[0]=_pY0;
    _pASD->Frame.pY[1]=_pY1;
}

void TNetDevice::fillChannelFilterSets(int _chan,int _type,int _aver_value,int _dif_value,int _lf_value,int _hf_value,int _iir_lf_value,int _iir_hf_value)
{
    HWNetChanFilter* _pFilter=&sets.chanFilter[_chan];
    _pFilter->type=_type;
    _pFilter->averValue=_aver_value;
    _pFilter->difValue=_dif_value;
    _pFilter->lfValue=_lf_value;
    _pFilter->hfValue=_hf_value;
    _pFilter->iir_lfValue=_iir_lf_value;
    _pFilter->iir_hfValue=_iir_hf_value;
    calcChannelFilterKoefs(_pFilter);
}

void TNetDevice::fillChannelSets(int _chan,int _frequency_hz,float _voltage_v,float _gain1_db,float _gain2_db,float _h_gain_db,float _v_gain_db,float _rotation,int _capacitor,int _input_type,int _ext_switch,int _balanceGain_t,float _balancePhase_dg,short int _Ku2DeltaX,short int _Ku2DeltaY)
{
    HWNetChan* _pChan=&sets.chan[_chan];
    _pChan->balanceGain_t=_balanceGain_t;
    _pChan->balancePhase_dg=_balancePhase_dg;
    _pChan->capacitor=_capacitor;
    _pChan->ext_switch=_ext_switch;
    _pChan->frequency_hz=_frequency_hz;
    _pChan->gain1_db=_gain1_db;
    _pChan->gain2_db=_gain2_db;
    _pChan->h_gain_db=_h_gain_db;
    _pChan->input_type=_input_type;
    _pChan->Ku2DeltaX=_Ku2DeltaX;
    _pChan->Ku2DeltaY=_Ku2DeltaY;
    _pChan->rotation=_rotation;
    _pChan->voltage_v=_voltage_v;
    _pChan->v_gain_db=_v_gain_db;
}
