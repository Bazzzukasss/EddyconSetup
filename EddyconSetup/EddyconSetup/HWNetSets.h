/* 
 * File:   HWSetsNet.h
 * Author: Baza
 *
 * Created on March 30, 2015, 4:30 PM
 */

#ifndef HWSETSNET_H
#define	HWSETSNET_H
#include "HWSets.h"


#define SDF_INDEX_X         (0)
#define SDF_INDEX_Y         (1)
#define SDF_INDEX_A         (2)
#define SDF_INDEX_ARAW      (3)
#define SDF_INDEX_OV1       (4)
#define SDF_INDEX_OV2       (5)
#define SDF_INDEX_RVTP      (6)
#define SDF_INDEX_DP        (7)
#define SDF_INDEX_ASD       (8)

#define SDF_X               (1<<SDF_INDEX_X)
#define SDF_Y               (1<<SDF_INDEX_Y)
#define SDF_A               (1<<SDF_INDEX_A)
#define SDF_ARAW            (1<<SDF_INDEX_ARAW)
#define SDF_OV1             (1<<SDF_INDEX_OV1)
#define SDF_OV2             (1<<SDF_INDEX_OV2)
#define SDF_RVTP            (1<<SDF_INDEX_RVTP)
#define SDF_DP              (1<<SDF_INDEX_DP)
#define SDF_ASD             (1<<SDF_INDEX_ASD)

#define SEND_DATA_FORMAT_FULL    (SDF_X|SDF_Y|SDF_A|SDF_ARAW|SDF_OV1|SDF_OV2|SDF_RVTP|SDF_DP|SDF_ASD)
struct HWNetSysSets{    
    int     meas_dly_t;
    int     adc_meas_dly_t;
    int     meas_len_t;
    int     corelation_deep;
    int     corelation_scale;
    int     meas_period_num;
    int     cur_freq;
    int     active_freq_number;
};
#define BALANSE_ITERATION_NUMBER        3
struct HWNetBalanseSets{
    int max_amp_value;
    int averaging_point_number;
    int amp_iteration_number[BALANSE_ITERATION_NUMBER];
    int ph_iteration_number[BALANSE_ITERATION_NUMBER];
    int beg_amp_value[BALANSE_ITERATION_NUMBER];
    float beg_ph_value[BALANSE_ITERATION_NUMBER];
};
#define MAX_CHAN_TO_MIX 4
struct HWNetMix{
    int chan;
    int chan_to_mix[MAX_CHAN_TO_MIX];
    int x_operation[MAX_CHAN_TO_MIX];
    int y_operation[MAX_CHAN_TO_MIX];
};
struct HWNetChanFilter
{
    int type;
    int averValue;
    int difValue;
    int lfValue;
    int hfValue;
    int iir_lfValue;
    int iir_hfValue;
    short int koefs_buf[2][IIR_FILTER_LEN];
};
struct HWNetPoint
{
        int             AmpI;
        float           X;
        float           Y;
        float           AmpF;
        float           Phase;
        float           DefVal;
        float           GapVal;
};
struct HWNetTester
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
struct HWNetMeasCursor
{
    int         Pos;
    int         Width;
    int         Type;
};
struct HWNetASDFrame
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
    int PointsCount;
};
struct HWNetChanASD
{
    HWNetASDFrame  Frame;
    int scale;
};
struct HWNetChanKalibrCurve
{
        int             PointsCount;
        int             Type;
        int             Units;
        HWNetPoint      Points[CALIBRATION_POINTS_NUM];
};
struct HWNetChan{
    int                 frequency_hz;
    float               voltage_v;
    float               gain1_db;
    float               gain2_db;
    float               h_gain_db;
    float               v_gain_db;
    float               rotation;
    int                 capacitor;
    int                 input_type;
    int                 ext_switch;
    int                 balanceGain_t;
    float               balancePhase_dg;
    int                 Ku2DeltaX;
    int                 Ku2DeltaY;
};
struct HWNetMainSets{
    int     max_Fsmp_hz;
    int     is_double_freq_mode;
    int     sync_type;
    int     rvtp_speed_index;
    int     testMode;
    int     measOnMm;
    int     rvtp_rounds_count;
    int     pulse_on_meas;
    int     rvtpAutoDetectMode;
    int     input_switch;
    int     ext_switch_type;
};
struct HWNetParams{
    int id;
    int fsmp;
    int dp_value;
    int dp_rounds;
};
struct HWNetSets{
    HWNetChan*              chan;
    HWNetChanASD*           chanASD;
    HWNetChanFilter*        chanFilter;    
    HWNetChanKalibrCurve*   chanKalibrCurve;
    HWNetMix*               mix;
    HWNetMeasCursor         measCursor;    
    HWNetBalanseSets        balanseSets;
    HWNetSysSets            sysSets;
    HWNetMainSets           mainSets;
};
#endif	/* HWSETSNET_H */

