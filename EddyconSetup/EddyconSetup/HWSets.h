/* 
 * File:   HWSets.h
 * Author: Baza
 *
 * Created on December 11, 2014, 10:42 AM
 */

#ifndef HWSETS_H
#define	HWSETS_H

#define CALIBRATION_POINTS_NUM  (128)
#define FRAME_POINTS_NUMBER   2
#define IIR_FILTER_LEN   5

struct HWTester
{
    int         AmpI;
    float       AmpF;
    double       Phase;
    float       DefVal;
    float       GapVal;
    int         Path;
    int         FlyPntX[3];
    int         FlyPntY[3];
    int         NebalanseAmp;
    int         TraktAmp;
    int         AmpRaw_prs;
    float       AmpFRaw;
    int         AmpIRaw;
    int         ASig;
    int         ANois;
    float       KSN;
    int         X;
    int         Y;
    int         CurChan;
    int         overflow[2];
    int         alarm[3];
    int         RPM;
    int         isAlarmed;
};
struct HWMeasCursor
{
    int         Pos;
    int         Width;
    int         Type;
    int         isReinit;
    int         isShow;
};
struct HWASDFrame
{
    int Type;
    int Radius;
    int BRadius;
    int Width;
    int Height;
    int Alfa;
    int Beta;
    int isSimetric;
    int Polarity;
    int pX[FRAME_POINTS_NUMBER];
    int pY[FRAME_POINTS_NUMBER];
    int Direction;
    int isAlarmed;
    int PointsCount;
};
struct HWASD
{
    HWASDFrame  Frame;
    HWASDFrame  Frame_calc;
    int scale;
};
struct HWFilter
{
    int type;
    int value1;
    int value2;
    short int koefs_buf[2][IIR_FILTER_LEN];
};

struct HWPoint
{
        int             AmpI;
        float           X;
        float           Y;
        float           AmpF;
        float           Phase;
        float           DefVal;
        float           GapVal;
};

struct HWKalibrCurve
{
        int             PointsCount;
        int             Type;
        int             Units;
        float           BaseGain1;
        float           BaseGain2;
        int             BaseGenVoltage;
        HWPoint         Points[CALIBRATION_POINTS_NUM];
        double          polinom_k[CALIBRATION_POINTS_NUM+1];
        int             K;
};
struct HWChan{
    int     frequency_hz;
    float   voltage_v;
    float   gain1_db;
    float   gain2_db;
    float   h_scale;
    float   v_scale;
    float   rotation;
    int     capacitor;
    int     input_type;
    int     balanceGain_t;
    float   balancePhase_dg;
    int     corelation_scan;
    int     meas_period_t;
    int     ext_switch;
    int     Ku2DeltaX;
    int     Ku2DeltaY;
    HWFilter  filter;
    int         traktAmp_prs;
    int         nebalanceAmp_prs;
    HWASD       ASD;
    HWKalibrCurve KalibrCurve;
};
struct HWMainSets{
    int     max_Fsmp_hz;
    int     is_double_freq_mode;
    int     sync_type;
    int     mix_type;
    int     input_switch;
    int     rvtp_speed_index;
    int     sweep_width;
    int     is_sound;
    int     testMode;
    int     measOnMm;
    int     rvtp_rounds_count;
    int     pulse_on_meas;
    float   ac_value;
    int     ac_width;
    int     rvtpAutoDetectMode;
    int     ext_switch_type;
};
#define RVTP_SPEED_NUMBER      10
struct HWRVTP{
    int     isButtonPresent;
    char    name[32];
    int     speed[RVTP_SPEED_NUMBER];
    int     speed_2[RVTP_SPEED_NUMBER*2];
};
struct HWRVTPSets{
    int number;
    HWRVTP* cur_rvtp;
    HWRVTP* rvtp;
};
#define MAX_CHAN_TO_MIX 4
struct HWMix{
    int chan;
    int chan_to_mix[MAX_CHAN_TO_MIX];
    int x_operation[MAX_CHAN_TO_MIX];
    int y_operation[MAX_CHAN_TO_MIX];
};
struct HWSysSets{    
    int     meas_dly_t;
    int     adc_meas_dly_t;
    int     meas_len_t;
    int     corelation_deep;
    int     corelation_scale;
    int     meas_period_num;
    int     cur_freq;
    int     active_freq_number;
};
struct HWSets{
    HWChan*         chan;
    HWMix*          mix;
    HWRVTPSets      rvtpSets;
    HWMainSets      mainSets;
    HWMeasCursor    measCursor;
    HWSysSets       sysSets;
    int     t_smp_2;
    int     f_smp_max;
    int     f_smp;
    int     max_meas_period_t;
    int     load_state;
};

struct HWDeviceSets{
    int     quartz_freq;
    int     max_Fsmp_hz;
    int     raw_buf_size_b;             //размер буфера под сырой сигнал
    int     raw_buf_max_amp;
    int     raw_buf_points_count;    
    int     spec_buf_points_count;    
    int     data_buf_size_b;            //размер буфера под прийом коррелированного сигнала
    int     data_buf_total_smp;         //максимальное количество измерений в буфере с данными
    int     data_buf_total_sweep_smp;   //максимальное количество измерений в буфере временных развёрток
    int     max_rvtp_rounds_number; //максимальное количество оборотов РВТП 
    int     freq_number;
    int     mix_number;
    int     meas_dly;
    int     filter_length;
    int     filter_w_function;
    int     filter_amp_koef;
    bool    isFilterTestMode;
    const char*   rvtp_list_file;
    const char*   temp_koef_file;
    float   meas_aver_koef;     
};

#define BALANSE_ITERATION_NUMBER        3

struct HWBalanseSets{
    int max_amp_value;
    int averaging_point_number;
    int amp_iteration_number[BALANSE_ITERATION_NUMBER];
    int ph_iteration_number[BALANSE_ITERATION_NUMBER];
    int beg_amp_value[BALANSE_ITERATION_NUMBER];
    float beg_ph_value[BALANSE_ITERATION_NUMBER];
};
struct HWBPoint
{
    int X;
    int Y;
    int A;
    float Ph;
    int Amp;
};
struct HWState
{
    bool isBridge;
    bool isRVTP;
    bool isCompensationProccess;
    bool isNeedToRefreshView;
    bool isControlModeStart;
    int rvtp_button_state;
    bool isFreez;
    volatile bool isNeedToResetDataBuffer;
};
#define DB_INIT         0
#define DB_CLEAR        1
#define DB_CLOSE        2
#define DB_CLEAR_FAST   3
#define DB_REWIND       4
#define DB_REWIND_ASD   5
struct HWRound
{
    int  Beg;
    int  Count;
};
struct HWRVTPRec
{
    int SyncCount;
    int RoundsCount;
    int BegSmp;
    int EndSmp;
    int SmpCount;
    HWRound* Rounds;
};

typedef short int HWDType;
struct HWDataSrc
{
    HWDType*    Buf;
    HWDType*    BegPtr;
    HWDType*    CurPtr;
    HWDType*    WrPtr;
    HWDType*    RdPtr;    
    HWDType*    SwRdPtr;
    HWDType*    ASDPtr;
    HWDType*    RvtpRdPtr;
    HWDType*    AcRdPtr;
};
struct HWDataSrc32
{
    int*        Buf;
    int*        BegPtr;
    int*        CurPtr;
    int*        WrPtr;
    int*        RdPtr;    
    int*        SwRdPtr;
    int*        ASDPtr;
    int*        RvtpRdPtr;
    int*        AcRdPtr;
};

struct HWDataBuf{
    bool                asd_enable;
    int                 cur_smp;                //текущий 
    int                 smp_count;              //всего
    int                 rvtp_smp_count;
    int                 rvtp_smp_count_last_round;
    int                 buffer_smp_count;       //сколько в принятом буфере
    volatile bool       isClearFlag;
    volatile bool       isFullClearFlag;
    volatile int        new_smp_count;          //сколько новых измерений (для ожидания)
    volatile bool       isNewMeas;
    int*                buffer;                 //буффер с принятыми данными
    short int*                buffer_x;                 //буффер с принятыми данными x
    short int*                buffer_y;                 //буффер с принятыми данными y
    short int*                buffer_amp;                 //буффер с принятыми данными amp
    short int*                buffer_dp_lo;                 //буффер с принятыми данными dp lsb
    short int*                buffer_dp_hi;                 //буффер с принятыми данными dp msb
    short int           overflow[2];
    HWDataSrc*          chanDataX;
    HWDataSrc*          chanDataY;
    HWDataSrc*          chanDataA;
    HWDataSrc*          chanDataARaw;
    HWDataSrc*          chanDataASD;
    HWDataSrc32         ASDData;
    HWDataSrc           SyncData;
    HWDataSrc32         DPData;
    HWDataSrc32         OverflowData[2];
    HWDType*            sweepBuf;
    HWDType*            sweepBufReview;
    unsigned short int* curRawAmp;
    HWDType*            curX;
    HWDType*            curY;
    int                 curDP;
    int                 curSYNC;                
    int                 is_overflow;
    volatile int        draw_smp;
    int                 sweep_draw_smp;
    int                 ac_draw_smp;
    volatile int        asd_smp;
    int                 send_smp_count;
    HWRVTPRec           RVTPRec;
    int                 cycles_count;
};


#endif	/* HWSETS_H */

