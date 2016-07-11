#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "UDPClient_EddyconC.h"
#include <QTimer>
#include "qcustomplot.h"
#include "TThread.h"
#define SERVER_PORT             0x6666
#define LOCALHOST_IP            "127.0.0.1"
#define EDDYCON_C_IP            "192.168.0.8"
#define EDDYCON_D4_IP           "192.168.0.4"
#define EDDYCON_D6_IP           "192.168.0.6"
#define SERVER_IP               EDDYCON_D6_IP
#define MAX_FREQ_NUMBER         32
#define MAX_MIX_NUMBER          32
#define RAW_BUF_SIZE_B          4096
#define RAW_BUF_MAX_AMP         8192
#define MAX_SWEEP_BUF_POINTS_COUNT  16384
#define SWEEP_BUF_POINTS_COUNT      1024
#define RAW_BUF_POINTS_COUNT    1024
#define DATA_BUF_TOTAL_SMP      100000
#define MAX_SIGNAL_AMP          4096

#define GET_DATA_TIMER_PERIOD   10
#define GET_TESTER_TIMER_PERIOD 500
#define DRAW_DATA_TIMER_PERIOD  40


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    inline void blockChangeSignal(){isChangeSignalActive=false;}
    inline void unblockChangeSignal(){isChangeSignalActive=true;}
    void drawInfo();
    void drawRawBuffer();
    void drawTester();

    void showSets();
    void showChannels();
    void showCurChannel();
    void updateChannels()
    {
        if(isChangeSignalActive)
        {
            fillChan(client->getNetDevice()->getCurChannel());
            showChannels();
        }
    }

    void fillChan(int _chan);
    void fillSets();
    void setPlotsRange(int _range_index,int _sweep_index);
    void setSweepRange(int _sweep_index);
    void setXYRange(int _range_index);
    bool connectToEddycon();
    void sendSTART();
    void sendALLsets();
private slots:
    void drawSignals();
    void LoadSettingsAction();
    void SaveSettingsAction();
    void setFormStyle();
    void drawEddyconData();
    void getEddyconData();
    void getEddyconTester();
    void on_pushButton_add_clicked();
    void on_pushButton_del_clicked();
    void on_pushButton_Refresh_clicked(){showChannels();showCurChannel();showSets();}
    void on_checkBox_getData_stateChanged(int arg1);
    void on_checkBox_getTester_stateChanged(int arg1);
    void on_spinBox_curChannel_valueChanged(int arg1);
    void on_tableWidget_channels_cellClicked(int row, int column);

    //command slots
    void on_pushButton_SRV_KILL_clicked()                          {client->send_CMD_SRV_KILL();}
    void on_pushButton_SRV_TST_clicked()                           {client->send_CMD_SRV_TEST();}
    void on_pushButton_SRV_REPEAT_clicked()                        {client->send_CMD_SRV_REPEAT();}
    void on_pushButton_SRV_VERSION_clicked()                       {client->send_CMD_SRV_VERSION();}
    void on_pushButton_SRV_RESTART_clicked()                       {client->send_CMD_SRV_RESTART();}
    void on_pushButton_SRV_BASH_clicked()                          {client->send_CMD_SRV_BASH();}

    void on_pushButton_RUN_START_clicked()                          {client->send_CMD_RUN_START();}
    void on_pushButton_RUN_STOP_clicked()                           {client->send_CMD_RUN_STOP();}
    void on_pushButton_RUN_CLEARING_clicked()                       {client->getNetDevice()->clearDataBuffers();client->send_CMD_RUN_CLEARING();}
    void on_pushButton_RUN_CREATECURVE_clicked()                    {client->send_CMD_RUN_CREATECURVE();}
    void on_pushButton_RUN_LOADING_clicked()                        {client->send_CMD_RUN_LOADING();}
    void on_pushButton_RUN_ADJUSTING_clicked()                      {client->send_CMD_RUN_ADJUSTING();}
    void on_pushButton_RUN_BALANSING_clicked()                      {client->send_CMD_RUN_BALANSING();}
    void on_pushButton_RUN_CENTERING_clicked()                      {client->send_CMD_RUN_CENTERING();}
    void on_pushButton_RUN_ROTATING_clicked()                       {client->send_CMD_RUN_ROTATING();}
    void on_pushButton_RUN_PREPAREING_clicked()                     {client->send_CMD_RUN_PREPARING();}

    void on_pushButton_GET_TESTER_clicked()                         {client->send_CMD_GET_TESTER(client->getNetDevice()->getCurChannel(),0,0);}
    void on_pushButton_GET_RAWDATA_clicked()                        {client->send_CMD_GET_RAWDATA();}
    void on_pushButton_GET_PARAMS_clicked()                         {client->send_CMD_GET_PARAMS();}
    void on_pushButton_GET_CHANNEL_NB_AMP_clicked()                 {client->send_CMD_GET_CHANNEL_NB_AMP();}
    void on_pushButton_GET_CHANNEL_TRAKT_AMP_clicked()              {client->send_CMD_GET_CHANNEL_TRAKT_AMP();}
    void on_pushButton_GET_CHANNEL_BALANSE_GAIN_clicked()           {client->send_CMD_GET_CHANNEL_BALANCE_GAIN();}
    void on_pushButton_GET_CHANNEL_BALANSE_PHASE_clicked()          {client->send_CMD_GET_CHANNEL_BALANCE_PHASE();}
    void on_pushButton_GET_CHANNEL_CAPASITOR_clicked()              {client->send_CMD_GET_CHANNEL_CAPASITOR();}
    void on_pushButton_GET_CHANNEL_ROTATION_clicked()               {client->send_CMD_GET_CHANNEL_ROTATION();}
    void on_pushButton_GET_CHANNEL_KU2DELTAX_clicked()              {client->send_CMD_GET_CHANNEL_KU2DELTAX();}
    void on_pushButton_GET_CHANNEL_KU2DELTAY_clicked()              {client->send_CMD_GET_CHANNEL_KU2DELTAY();}
    void on_pushButton_GET_XYDATA_clicked()                         {client->send_CMD_GET_XYDATA(SEND_DATA_FORMAT_FULL,0);}
    void on_pushButton_ALL_XY_DATA_clicked()                        {client->send_CMD_GET_XYDATA(SEND_DATA_FORMAT_FULL,1);}

    void on_pushButton_SET_CHANNEL_KALIBRCURVE_clicked()            {client->send_CMD_SET_CHANNEL_KALIBRCURVE(client->getNetDevice()->getCurChannel());}
    void on_pushButton_SET_CHANNEL_SETS_clicked()                   {client->send_CMD_SET_CHANNEL_SETS(client->getNetDevice()->getCurChannel(),1);}
    void on_pushButton_SET_CHANNEL_FILTER_clicked()                 {client->send_CMD_SET_CHANNEL_FILTER(client->getNetDevice()->getCurChannel());}
    void on_pushButton_SET_CHANNEL_ASD_clicked()                    {client->send_CMD_SET_CHANNEL_ASD(client->getNetDevice()->getCurChannel());}
    void on_pushButton_SET_MIX_clicked()                            {client->send_CMD_SET_MIX(0);}
    void on_pushButton_SET_MEASCURSOR_clicked()                     {client->send_CMD_SET_MEASCURSOR();}
    void on_pushButton_SET_MAINSETS_clicked()                       {client->send_CMD_SET_MAINSETS();}
    void on_pushButton_SET_SYSSETS_clicked()                        {client->send_CMD_SET_SYSSETS();}
    void on_pushButton_SET_BALSETS_clicked()                        {client->send_CMD_SET_BALSETS();}
    void on_pushButton_SET_DB_SEND_SMP_COUNT_clicked()              {client->send_CMD_SET_DATABUFFER_SEND_SMP_COUNT(-1);}
    void on_pushButton_START_XY_clicked();
    void on_pushButton_START_RAW_clicked();
    void on_pushButton_STOP_clicked();
    void on_pushButton_SET_ALL_clicked();


    //Change channels params slots
    void on_spinBox_chanFreq_valueChanged(int arg1)                 {cout<<arg1<<endl;updateChannels();}
    void on_comboBox_chanVoltage_currentIndexChanged(int index)     {cout<<index<<endl;updateChannels();}
    void on_doubleSpinBox_chanPreGain_valueChanged(double arg1)     {cout<<arg1<<endl;updateChannels();}
    void on_doubleSpinBox_chanGain_valueChanged(double arg1)        {cout<<arg1<<endl;updateChannels();}
    void on_doubleSpinBox_chanHGain_valueChanged(double arg1)       {cout<<arg1<<endl;updateChannels();}
    void on_doubleSpinBox_chanVGain_valueChanged(double arg1)       {cout<<arg1<<endl;updateChannels();}
    void on_doubleSpinBox_chanRotation_valueChanged(double arg1)    {cout<<arg1<<endl;updateChannels();}
    void on_spinBox_chanCapacitor_valueChanged(int arg1)            {cout<<arg1<<endl;updateChannels();}
    void on_comboBox_chanInput_currentIndexChanged(int index)       {cout<<index<<endl;updateChannels();}
    void on_spinBox_chanSwitcher_valueChanged(int arg1)             {cout<<arg1<<endl;updateChannels();}
    void on_spinBox_chanBalGain_valueChanged(int arg1)              {cout<<arg1<<endl;updateChannels();}
    void on_doubleSpinBox_chanBalPh_valueChanged(double arg1)       {cout<<arg1<<endl;updateChannels();}
    void on_spinBox_chanKu2DeltaX_valueChanged(int arg1)            {cout<<arg1<<endl;updateChannels();}
    void on_spinBox_chanKu2DeltaY_valueChanged(int arg1)            {cout<<arg1<<endl;updateChannels();}
    void on_comboBox_filterType_currentIndexChanged(int index)      {cout<<index<<endl;updateChannels();}
    void on_spinBox_filterAverValue_valueChanged(int arg1)          {cout<<arg1<<endl;updateChannels();}
    void on_spinBox_filterLFValue_valueChanged(int arg1)            {cout<<arg1<<endl;updateChannels();}
    void on_spinBox_filterIIR_LFValue_valueChanged(int arg1)        {cout<<arg1<<endl;updateChannels();}
    void on_comboBox_asdType_currentIndexChanged(int index)         {cout<<index<<endl;updateChannels();}
    void on_spinBox_asdRadius_valueChanged(int arg1)                {cout<<arg1<<endl;updateChannels();}
    void on_spinBox_asdBRadius_valueChanged(int arg1)               {cout<<arg1<<endl;updateChannels();}
    void on_spinBox_asdWidth_valueChanged(int arg1)                 {cout<<arg1<<endl;updateChannels();}
    void on_spinBox_asdHeight_valueChanged(int arg1)                {cout<<arg1<<endl;updateChannels();}
    void on_spinBox_asdAlfa_valueChanged(int arg1)                  {cout<<arg1<<endl;updateChannels();}
    void on_spinBox_asdBeta_valueChanged(int arg1)                  {cout<<arg1<<endl;updateChannels();}
    void on_spinBox_asdX0_valueChanged(int arg1)                    {cout<<arg1<<endl;updateChannels();}
    void on_spinBox_asdY0_valueChanged(int arg1)                    {cout<<arg1<<endl;updateChannels();}
    void on_spinBox_asdX1_valueChanged(int arg1)                    {cout<<arg1<<endl;updateChannels();}
    void on_spinBox_asdY1_valueChanged(int arg1)                    {cout<<arg1<<endl;updateChannels();}
    void on_comboBox_asdIsSimetric_currentIndexChanged(int index)   {cout<<index<<endl;updateChannels();}
    void on_comboBox_asdPolarity_currentIndexChanged(int index)     {cout<<index<<endl;updateChannels();}
    void on_comboBox_asdDirection_currentIndexChanged(int index)    {cout<<index<<endl;updateChannels();}
    void on_spinBox_filterDifValue_valueChanged(int arg1)           {cout<<arg1<<endl;updateChannels();}
    void on_spinBox_filterHFValue_valueChanged(int arg1)            {cout<<arg1<<endl;updateChannels();}
    void on_spinBox_filterIIR_HFValue_valueChanged(int arg1)        {cout<<arg1<<endl;updateChannels();}
    //Change sets slots
    void on_comboBox_syncType_currentIndexChanged(int index)        {cout<<index<<endl;fillSets();}
    void on_comboBox_inputType_currentIndexChanged(int index)       {cout<<index<<endl;fillSets();}
    void on_comboBox_extSwitchType_currentIndexChanged(int index)       {cout<<index<<endl;fillSets();}
    void on_spinBox_maxFsmp_valueChanged(int arg1)                  {cout<<arg1<<endl;fillSets();}
    void on_comboBox_testMode_currentIndexChanged(int index)        {cout<<index<<endl;fillSets();}
    void on_spinBox_measOnmm_valueChanged(int arg1)                 {cout<<arg1<<endl;fillSets();}
    void on_spinBox_pulseOnMeas_valueChanged(int arg1)              {cout<<arg1<<endl;fillSets();}
    void on_comboBox_isDoubleFreqMode_currentIndexChanged(int index){cout<<index<<endl;fillSets();}
    void on_comboBox_rvtpSpeed_currentIndexChanged(int index)       {cout<<index<<endl;fillSets();}
    void on_spinBox_roundsCount_valueChanged(int arg1)              {cout<<arg1<<endl;fillSets();}
    void on_comboBox_rvtpAutodetect_currentIndexChanged(int index)  {cout<<index<<endl;fillSets();}
    //void on_spinBox_curFreq_valueChanged(int arg1)                  {cout<<arg1<<endl;fillSets();}
    //void on_spinBox_activeFreqNum_valueChanged(int arg1)            {cout<<arg1<<endl;fillSets();}
    void on_spinBox_measDly_valueChanged(int arg1)                  {cout<<arg1<<endl;fillSets();}
    void on_spinBox_adcMeasDly_valueChanged(int arg1)               {cout<<arg1<<endl;fillSets();}
    void on_spinBox_measLen_valueChanged(int arg1)                  {cout<<arg1<<endl;fillSets();}
    void on_spinBox_corelationDeep_valueChanged(int arg1)           {cout<<arg1<<endl;fillSets();}
    void on_spinBox_corelationScale_valueChanged(int arg1)          {cout<<arg1<<endl;fillSets();}
    void on_spinBox_measPeriodNum_valueChanged(int arg1)            {cout<<arg1<<endl;fillSets();}

    void on_comboBox_swepWidth_currentIndexChanged(int index);
    void on_horizontalSlider_Scale_valueChanged(int value);
    void on_lineEdit_Bush_textChanged(const QString &arg1);
    void on_pushButton_RECONNECT_clicked();


    void on_comboBox_DataType_currentIndexChanged(int index);
    /*
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
*/
    //void on_comboBox_viewType_currentIndexChanged(int index);
    //void on_comboBox_multyType_currentIndexChanged(int index);
    void on_customPlot_XY_mousePressed(QMouseEvent *event);



    void on_spinBox_get_data_interval_valueChanged(int arg1);

private:
    TNetClientThread* prepare_data_thread;
    TNetClientThread* get_net_data_thread;
    bool isChangeSignalActive;
    Ui::MainWindow *ui;
    UDPClient_EddyconC *client;
    TNetDevice*  netDevice;
    QTimer* timer_getdata;
    QTimer* timer_gettester;
    QTimer* timer_drawdata;
    //int viewType;
    //int multyType;
    QList<QColor> colorsArray;
};

#endif // MAINWINDOW_H
