#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "HWNetSets.h"
#include <QString>




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    //infoString = new QString("Info string");
    isChangeSignalActive=true;
    ui->setupUi(this);
    netDevice=new TNetDevice(MAX_FREQ_NUMBER,MAX_MIX_NUMBER,RAW_BUF_POINTS_COUNT,RAW_BUF_MAX_AMP,DATA_BUF_TOTAL_SMP,MAX_SWEEP_BUF_POINTS_COUNT);
    client = new UDPClient_EddyconC(SERVER_IP,SERVER_PORT,netDevice);



    timer_getdata = new QTimer(this);
    timer_drawdata = new QTimer(this);
    timer_gettester = new QTimer(this);

    connect(timer_getdata, SIGNAL(timeout()), this, SLOT(getEddyconData()));
    connect(timer_drawdata, SIGNAL(timeout()), this, SLOT(drawEddyconData()));
    connect(timer_gettester, SIGNAL(timeout()), this, SLOT(getEddyconTester()));

    connect(ui->customPlot_XY,SIGNAL(mousePress(QMouseEvent*)),this,SLOT(on_customPlot_XY_mousePressed(QMouseEvent*)));

    //timer_getdata->start(GET_DATA_TIMER_PERIOD);
    timer_drawdata->start(DRAW_DATA_TIMER_PERIOD);
    //timer_gettester->start(GET_TESTER_TIMER_PERIOD);

    ui->spinBox_curChannel->setMaximum(client->getNetDevice()->getChannelCount()-1);

    ui->customPlot_RAW->setBackground(QBrush(QColor(0xee,0xee,0xee)));
    ui->customPlot_RAW->addGraph();
    ui->customPlot_RAW->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    //ui->customPlot_RAW->xAxis->setLabel("i");
    //ui->customPlot_RAW->yAxis->setLabel("Amp");
    ui->customPlot_RAW->yAxis->setRange(-client->getNetDevice()->getRawBuffMaxAmp(), client->getNetDevice()->getRawBuffMaxAmp());
    ui->customPlot_RAW->xAxis->setRange(0, RAW_BUF_POINTS_COUNT);
    ui->customPlot_RAW->axisRect()->setupFullAxesBox();


    //ui->customPlot_XY->addGraph();
    //ui->customPlot_XY->graph(0)->setBrush(QBrush(QColor(255, 0, 0, 20)));
    QCPCurve* godograph[MAX_FREQ_NUMBER];
    ui->customPlot_XY->setBackground(QBrush(QColor(0xee,0xee,0xee)));
    colorsArray << Qt::blue << Qt::red << Qt::green << Qt::black << Qt::yellow << Qt::gray << Qt::magenta << Qt::cyan;
    for(int chan=0;chan<MAX_FREQ_NUMBER;chan++)
    {
        godograph[chan]=new QCPCurve(ui->customPlot_XY->xAxis, ui->customPlot_XY->yAxis);
        ui->customPlot_XY->addPlottable(godograph[chan]);
        ui->customPlot_XY->plottable(chan)->setPen(QPen(colorsArray[chan&7]));
    }

    //ui->customPlot_XY->xAxis->setLabel("x");
    //ui->customPlot_XY->yAxis->setLabel("y");
    ui->customPlot_XY->axisRect()->setupFullAxesBox();
    ui->customPlot_XY->xAxis->setTickLabels(false);
    ui->customPlot_XY->yAxis->setTickLabels(false);
    ui->customPlot_XY->axisRect()->setAutoMargins(QCP::msNone);
    ui->customPlot_XY->axisRect()->setMargins(QMargins(1,1,1,1));

    ui->customPlot_SWEEP->setBackground(QBrush(QColor(0xee,0xee,0xee)));
    ui->customPlot_SWEEP->addGraph();
    ui->customPlot_SWEEP->addGraph();
    ui->customPlot_SWEEP->addGraph();
    ui->customPlot_SWEEP->addGraph();
    ui->customPlot_SWEEP->graph(0)->setPen(QPen(Qt::blue));
    ui->customPlot_SWEEP->graph(1)->setPen(QPen(Qt::blue));
    ui->customPlot_SWEEP->graph(2)->setPen(QPen(Qt::darkGreen));
    ui->customPlot_SWEEP->graph(3)->setPen(QPen(Qt::darkGray));
    //ui->customPlot_SWEEP->graph(3)->setLineStyle(QCPGraph::lsImpulse);

    ui->customPlot_SWEEP->axisRect()->setupFullAxesBox();
    ui->customPlot_SWEEP->yAxis->setTickLabels(false);
    //ui->customPlot_SWEEP->xAxis->setVisible(false);
    ui->customPlot_SWEEP->axisRect()->setAutoMargins(QCP::msNone);
    ui->customPlot_SWEEP->axisRect()->setMargins(QMargins(1,1,1,1));


    ui->customPlot_RVTP->setBackground(QBrush(QColor(0xee,0xee,0xee)));
    ui->customPlot_RVTP->addGraph();
    ui->customPlot_RVTP->addGraph();
    ui->customPlot_RVTP->graph(0)->setPen(QPen(Qt::black));
    ui->customPlot_RVTP->axisRect()->setupFullAxesBox();
    ui->customPlot_RVTP->yAxis->setTickLabels(false);
    ui->customPlot_RVTP->axisRect()->setAutoMargins(QCP::msNone);
    ui->customPlot_RVTP->axisRect()->setMargins(QMargins(1,1,1,1));

    ui->customPlot_BIT->setBackground(QBrush(QColor(0xee,0xee,0xee)));
    ui->customPlot_BIT->addGraph();
    ui->customPlot_BIT->addGraph();
    ui->customPlot_BIT->addGraph();
    ui->customPlot_BIT->addGraph();
    ui->customPlot_BIT->graph(0)->setPen(QPen(QBrush(Qt::darkGray),3,Qt::SolidLine));
    ui->customPlot_BIT->graph(1)->setPen(QPen(QBrush(Qt::red),3,Qt::SolidLine));
    ui->customPlot_BIT->graph(2)->setPen(QPen(QBrush(Qt::darkRed),3,Qt::SolidLine));
    ui->customPlot_BIT->graph(3)->setPen(QPen(QBrush(Qt::darkYellow),3,Qt::SolidLine));

    //ui->customPlot_BIT->graph(3)->setLineStyle(QCPGraph::lsImpulse);


    ui->customPlot_BIT->xAxis->setVisible(false);
    //ui->customPlot_BIT->yAxis->setLabel("RVTP,OV1/2,ASD");
    ui->customPlot_BIT->yAxis->setTickLabels(false);
    ui->customPlot_BIT->yAxis->setAutoTickStep(false);
    ui->customPlot_BIT->yAxis->setTickStep(1);
    ui->customPlot_BIT->axisRect()->setAutoMargins(QCP::msNone);
    ui->customPlot_BIT->axisRect()->setMargins(QMargins(1,1,1,1));

    client->getNetDevice()->setViewType(VT_XY);
    client->getNetDevice()->setMultyType(MT_X);
    setSweepRange(ui->comboBox_swepWidth->currentIndex());
    setXYRange(ui->horizontalSlider_Scale->value());


    client->getNetDevice()->setSweepWidth(pow(2,ui->comboBox_swepWidth->currentIndex()+7));
    showCurChannel();
    showChannels();
    showSets();
    connectToEddycon();
    client->getNetDevice()->setInfoReady(true);
    //setFormStyle();

    connect(ui->actionLoad_setting, SIGNAL(triggered()), this, SLOT(LoadSettingsAction()));
    connect(ui->actionSave_settings, SIGNAL(triggered()), this, SLOT(SaveSettingsAction()));

    prepare_data_thread=new TNetClientThread(client,50);
    connect(prepare_data_thread,SIGNAL(ProcedureFinished()),this,SLOT(drawSignals()));
    prepare_data_thread->start();

}
MainWindow::~MainWindow()
{
    delete ui;
    delete client;
    delete netDevice;
    delete prepare_data_thread;
}
void MainWindow::setFormStyle()
{
    // Скрываем элементы управления и рамку окна...
    //setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);

    // Включаем прозрачность главной формы...
    setAttribute(Qt::WA_TranslucentBackground);

    // Задаём параметры прозрачности...
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect -> setBlurRadius(9.0);
    shadowEffect -> setColor(QColor(0, 0, 0, 160));
    shadowEffect -> setOffset(4.0);
    ui->centralWidget->setGraphicsEffect(shadowEffect);
}
void MainWindow::setPlotsRange(int _range_index,int _sweep_index)
{
    setSweepRange(_sweep_index);
    setXYRange(_range_index);
    ui->customPlot_XY->replot();
    ui->customPlot_SWEEP->replot();
    ui->customPlot_BIT->replot();
    ui->customPlot_RAW->replot();

}
void MainWindow::setSweepRange(int _sweep_index)
{
    double range=pow(2,_sweep_index+7);
    client->getNetDevice()->setSweepWidth(range);
    ui->customPlot_SWEEP->xAxis->setRange(0, range);
    ui->customPlot_BIT->xAxis->setRange(0, range);
    if(client->getNetDevice()->getViewType()==VT_MULTY)
        ui->customPlot_XY->yAxis->setRange(0,range);
}
void MainWindow::setXYRange(int _range_index)
{
    double range=pow(2,_range_index);
    client->getNetDevice()->setXYRange(range);
    if(ui->comboBox_DataType->currentIndex()==0)
        ui->customPlot_RAW->yAxis->setRange(0,range);
    else
        ui->customPlot_RAW->yAxis->setRange(-range,range);

    ui->customPlot_SWEEP->yAxis->setRange(-range,range);
    ui->customPlot_RVTP->yAxis->setRange(-range,range);
    ui->customPlot_BIT->yAxis->setRange(0,4.2);
    ui->customPlot_XY->xAxis->setRange(-range,range);
    if(client->getNetDevice()->getViewType()==VT_XY)
        ui->customPlot_XY->yAxis->setRange(-range,range);
}


void MainWindow::drawInfo()
{
    ui->textEdit_Info->setText(client->getNetDevice()->getInfoText());
}

void MainWindow::drawTester()
{
    //HWNetTester* pTester=client->getNetDevice()->getTester();
    //QString str;
    //str.sprintf("%2d) <font color=\"#CC0000\"> AF=%5.2fV / %6d </font><font color=\"#666666\">(an=%5d as=%5d ksn=%5.2fdB)</font>  <font color=\"#CC0000\">ARaw=%5.2fV / %5d / %3d%%</font> <font color=\"#666666\">(t=%3d%% nb=%3d%%)</font> <font color=\"#CC0000\">Ph=%7.2fdg  D=%5.2fmm</font>   <font color=\"#009900\">RPM=%4d</font> <font color=\"#000099\">Path=%5d</font> ",pTester->CurChan,pTester->AmpF,pTester->AmpI,pTester->ANois,pTester->ASig,pTester->KSN,pTester->AmpFRaw,pTester->AmpIRaw,pTester->AmpRaw_prs,pTester->TraktAmp,pTester->NebalanseAmp,pTester->Phase,pTester->DefVal,pTester->RPM,pTester->Path);
    ui->label_tester->setText(client->getNetDevice()->getTesterString());
}

void MainWindow::drawRawBuffer()
{
    int count=client->getNetDevice()->getRawBuffPointsCount();
    if(count>0)
    {
        ui->customPlot_RAW->xAxis->setRange(0, count);
        ui->customPlot_RAW->graph(0)->setData(client->getNetDevice()->getSweepBuf(), client->getNetDevice()->getRawBuf());
        ui->customPlot_RAW->replot();
        ui->customPlot_RAW->graph(0)->clearData();
    }
}
#if 1
void MainWindow::drawSignals()
{
    TNetDevice* pDev=client->getNetDevice();
    QMutexLocker locker(&pDev->mutex);
    int chan_count;
    static int old_chan_count=0;

    chan_count=pDev->getChannelCount();
    ui->customPlot_SWEEP->graph(0)->setData(pDev->getSweepBuf(), pDev->getDrawSweepXBuf());
    ui->customPlot_SWEEP->graph(1)->setData(pDev->getSweepBuf(), pDev->getDrawSweepYBuf());
    ui->customPlot_SWEEP->graph(2)->setData(pDev->getSweepBuf(), pDev->getDrawSweepARawBuf());
    ui->customPlot_SWEEP->graph(3)->setData(pDev->getSweepBuf(), pDev->getDrawSweepRVTPBufS());
    ui->customPlot_RVTP->graph(0)->setData(pDev->getSweepBuf(), pDev->getDrawRvtpYBuf());
    ui->customPlot_BIT->graph(0)->setData(pDev->getSweepBuf(), pDev->getDrawSweepRVTPBuf());
    ui->customPlot_BIT->graph(1)->setData(pDev->getSweepBuf(), pDev->getDrawSweepOV1Buf());
    ui->customPlot_BIT->graph(2)->setData(pDev->getSweepBuf(), pDev->getDrawSweepOV2Buf());
    ui->customPlot_BIT->graph(3)->setData(pDev->getSweepBuf(), pDev->getDrawSweepASDBuf());
    for(int chan=0;chan<chan_count;chan++)
    {
        QCPCurve* godograph=reinterpret_cast<QCPCurve*>(ui->customPlot_XY->plottable(chan));
        if(pDev->getViewType()==VT_XY)
            godograph->setData(pDev->getDrawXBuf(chan),pDev->getDrawYBuf(chan));
        else
        {
            switch(pDev->getMultyType())
            {
                case MT_X:  godograph->setData(pDev->getDrawXBuf(chan),pDev->getSweepBuf());break;
                case MT_Y:  godograph->setData(pDev->getDrawYBuf(chan),pDev->getSweepBuf());break;
                case MT_A:  godograph->setData(pDev->getDrawARawBuf(chan),pDev->getSweepBuf());break;
            }
        }
    }

    ui->customPlot_SWEEP->replot();
    ui->customPlot_BIT->replot();
    ui->customPlot_XY->replot();
    ui->customPlot_RVTP->replot();

    ///Clearing
    if(old_chan_count>chan_count)
    {
        old_chan_count=chan_count;
        for(int chan=0;chan<chan_count;chan++)
        {
            QCPCurve* godograph=reinterpret_cast<QCPCurve*>(ui->customPlot_XY->plottable(chan));
            godograph->clearData();
        }
    }
/*
    ui->customPlot_SWEEP->graph(0)->clearData();
    ui->customPlot_SWEEP->graph(1)->clearData();
    ui->customPlot_SWEEP->graph(2)->clearData();
    ui->customPlot_SWEEP->graph(3)->clearData();
    ui->customPlot_RVTP->graph(0)->clearData();
    ui->customPlot_BIT->graph(0)->clearData();
    ui->customPlot_BIT->graph(1)->clearData();
    ui->customPlot_BIT->graph(2)->clearData();
    ui->customPlot_BIT->graph(3)->clearData();
    */
}
#else
void MainWindow::drawSignals()
{
    //int Cntr=client->getNetDevice()->getXYRange()/2;
    double t,w,center_delta_x,center_delta_y,center_delta;
    int cur_freq=client->getNetDevice()->getCurChannel();
    int chan_count=client->getNetDevice()->getChannelCount();
    QVector<double> XBuf[chan_count],YBuf[chan_count],ARawBuf[chan_count],rvtpXBuf,rvtpYBuf,sweepXBuf,sweepYBuf,sweepARawBuf,sweepRVTPBuf,sweepRVTPBufS,sweepOV1BufS,sweepOV2BufS,sweepASDBufS,sweepOV1Buf,sweepOV2Buf,sweepASDBuf;

    int smp_to_draw=client->getNetDevice()->getSweepSmpCountToDraw();
    int rvtp_smp_to_draw=client->getNetDevice()->getRvtpSmpCountToDraw();
    w=client->getNetDevice()->getXYRange()*2;
    t=w/(chan_count+1);
    //qDebug()<<w<<" "<<t<<endl;

    if(smp_to_draw>0)
    {
        //client->getNetDevice()->calcSpectr();
        //cout<<"sweep_smp_to_draw="<<smp_to_draw<<endl;
        client->getNetDevice()->setInfoReady(true);
        for(int chan=0;chan<chan_count;chan++)
        {
            XBuf[chan].resize(smp_to_draw);
            YBuf[chan].resize(smp_to_draw);
            ARawBuf[chan].resize(smp_to_draw);
        }
        if(rvtp_smp_to_draw)
        {
            rvtpXBuf.resize(rvtp_smp_to_draw);
            rvtpYBuf.resize(rvtp_smp_to_draw);
            ui->customPlot_RVTP->xAxis->setRange(0, rvtp_smp_to_draw);
        }
        sweepXBuf.resize(smp_to_draw);
        sweepYBuf.resize(smp_to_draw);
        sweepARawBuf.resize(smp_to_draw);
        sweepRVTPBuf.resize(smp_to_draw);
        sweepRVTPBufS.resize(smp_to_draw);
        sweepOV1BufS.resize(smp_to_draw);
        sweepOV2BufS.resize(smp_to_draw);
        sweepASDBufS.resize(smp_to_draw);
        sweepOV1Buf.resize(smp_to_draw);
        sweepOV2Buf.resize(smp_to_draw);
        sweepASDBuf.resize(smp_to_draw);
        for(int i=0;i<smp_to_draw;i++)
        {
            for(int chan=0;chan<chan_count;chan++)
            {
                center_delta=(chan-chan_count/2)*t;
                if(chan_count%2==0)
                {
                    center_delta+=t/2;
                }
                if(client->getNetDevice()->getViewType()==VT_XY)
                {
                    center_delta_x=center_delta;
                    center_delta_y=0;
                }
                else
                {
                    center_delta_x=center_delta_y=center_delta;
                }
                XBuf[chan][i]=client->getNetDevice()->getSweepChanXBuf(chan,i)+center_delta_x;
                YBuf[chan][i]=client->getNetDevice()->getSweepChanYBuf(chan,i)+center_delta_y;
                ARawBuf[chan][i]=client->getNetDevice()->getSweepChanARawBuf(chan,i)+center_delta_y;
            }
            sweepXBuf[i]=client->getNetDevice()->getSweepChanXBuf(cur_freq,i);//+Cntr;
            sweepYBuf[i]=client->getNetDevice()->getSweepChanYBuf(cur_freq,i);//-Cntr;
            sweepARawBuf[i]=client->getNetDevice()->getSweepChanARawBuf(cur_freq,i);
            sweepRVTPBufS[i]=client->getNetDevice()->getSweepRVTPBuf(i)*(1<<17)-(1<<16);
            sweepRVTPBuf[i]=client->getNetDevice()->getSweepRVTPBuf(i)+0.1;
            sweepOV1Buf[i]=client->getNetDevice()->getSweepOV1Buf(i)+1.1;
            sweepOV2Buf[i]=client->getNetDevice()->getSweepOV2Buf(i)+2.1;
            sweepASDBuf[i]=client->getNetDevice()->getSweepASDBuf(i)+3.1;
        }
        for(int j=0;j<rvtp_smp_to_draw;j++)
        {
            rvtpXBuf[j]=client->getNetDevice()->getRvtpChanXBuf(cur_freq,j);
            rvtpYBuf[j]=client->getNetDevice()->getRvtpChanYBuf(cur_freq,j);
        }
        //client->getNetDevice()->incXYSmpDrawn(smp_to_draw);

        ui->customPlot_SWEEP->graph(0)->setData(client->getNetDevice()->getSweepBuf(), sweepXBuf);
        ui->customPlot_SWEEP->graph(1)->setData(client->getNetDevice()->getSweepBuf(), sweepYBuf);
        ui->customPlot_SWEEP->graph(2)->setData(client->getNetDevice()->getSweepBuf(), sweepARawBuf);
        ui->customPlot_SWEEP->graph(3)->setData(client->getNetDevice()->getSweepBuf(), sweepRVTPBufS);


        ui->customPlot_RVTP->graph(0)->setData(client->getNetDevice()->getSweepBuf(), rvtpYBuf);

        ui->customPlot_BIT->graph(0)->setData(client->getNetDevice()->getSweepBuf(), sweepRVTPBuf);
        ui->customPlot_BIT->graph(1)->setData(client->getNetDevice()->getSweepBuf(), sweepOV1Buf);
        ui->customPlot_BIT->graph(2)->setData(client->getNetDevice()->getSweepBuf(), sweepOV2Buf);
        ui->customPlot_BIT->graph(3)->setData(client->getNetDevice()->getSweepBuf(), sweepASDBuf);
        for(int chan=0;chan<chan_count;chan++)
        {
            QCPCurve* godograph=reinterpret_cast<QCPCurve*>(ui->customPlot_XY->plottable(chan));
            if(client->getNetDevice()->getViewType()==VT_XY)
                godograph->setData(XBuf[chan],YBuf[chan]);
            else
            {
                switch(client->getNetDevice()->getMultyType())
                {
                    case MT_X:  godograph->setData(XBuf[chan],client->getNetDevice()->getSweepBuf());break;
                    case MT_Y:  godograph->setData(YBuf[chan],client->getNetDevice()->getSweepBuf());break;
                    case MT_A:  godograph->setData(ARawBuf[chan],client->getNetDevice()->getSweepBuf());break;
                }
            }
        }
        ui->customPlot_SWEEP->replot();
        ui->customPlot_BIT->replot();
        ui->customPlot_XY->replot();
        ui->customPlot_RVTP->replot();

        ///Clearing
        for(int chan=0;chan<chan_count;chan++)
        {
            XBuf[chan].clear();
            YBuf[chan].clear();
            ARawBuf[chan].clear();
            QCPCurve* godograph=reinterpret_cast<QCPCurve*>(ui->customPlot_XY->plottable(chan));
            godograph->clearData();
        }
        rvtpXBuf.clear();
        rvtpYBuf.clear();
        sweepXBuf.clear();
        sweepYBuf.clear();
        sweepARawBuf.clear();
        sweepRVTPBufS.clear();
        sweepOV1BufS.clear();
        sweepOV2BufS.clear();
        sweepASDBufS.clear();
        sweepRVTPBuf.clear();
        sweepOV1Buf.clear();
        sweepOV2Buf.clear();
        sweepASDBuf.clear();

        ui->customPlot_SWEEP->graph(0)->clearData();
        ui->customPlot_SWEEP->graph(1)->clearData();
        ui->customPlot_SWEEP->graph(2)->clearData();
        ui->customPlot_SWEEP->graph(3)->clearData();

        ui->customPlot_RVTP->graph(0)->clearData();

        ui->customPlot_BIT->graph(0)->clearData();
        ui->customPlot_BIT->graph(1)->clearData();
        ui->customPlot_BIT->graph(2)->clearData();
        ui->customPlot_BIT->graph(3)->clearData();
    }
}
#endif



void MainWindow::getEddyconData()
{
    if(ui->comboBox_DataType->currentIndex()>0)
        client->send_CMD_GET_RAWDATA();
    else
    {
        if(ui->checkBox_isAllData->checkState()==Qt::Checked)
            client->send_CMD_GET_XYDATA(SEND_DATA_FORMAT_FULL,1);
        else
            client->send_CMD_GET_XYDATA(SEND_DATA_FORMAT_FULL,0);
    }
}
void MainWindow::getEddyconTester()
{
    client->send_CMD_GET_TESTER(client->getNetDevice()->getCurChannel(),0,0);
}
bool MainWindow::connectToEddycon()
{
    QString ip=ui->comboBox_ip->currentText();
    bool res=client->reconnect(ip,SERVER_PORT);
    if(res)
        client->getNetDevice()->setBoardIP(ip);
    else
        client->getNetDevice()->setBoardIP("---");
    return res;
}

void MainWindow::drawEddyconData()
{
    if(client->getNetDevice()->getRawBuffReady())
    {
        drawRawBuffer();
        client->getNetDevice()->setRawBuffReady(0);
    }
    if(client->getNetDevice()->getTesterReady())
    {
        drawTester();
        client->getNetDevice()->setTesterReady(false);
    }
    if(client->getNetDevice()->getChannelsReady())
    {
        showChannels();
        showCurChannel();
        client->getNetDevice()->setChannelsReady(false);
    }
    if(client->getNetDevice()->getInfoReady())
    {
        drawInfo();
        client->getNetDevice()->setInfoReady(false);
    }
    /*
    if(client->getNetDevice()->getSignalsReady())
    {
        drawSignals();
        client->getNetDevice()->setSignalsReady(false);
    }
    */
}

void MainWindow::showChannels()
{
    TNetDevice* pNetDevice=client->getNetDevice();
    HWNetChan* pChan;
    HWNetASDFrame* pAsdFrame;
    HWNetChanFilter* pFilter;
    QTableWidgetItem *item;
    QColor colors[]={QColor(255, 255, 155),QColor(155, 255, 179),QColor(155, 222, 255),QColor(255, 155, 155)};
    int val=0;
    int c=0;

    int max_freq_num=pNetDevice->getChannelCount();

    ui->tableWidget_channels->clear();
    ui->tableWidget_channels->setHorizontalHeaderLabels(QString("Color;Frq,kHz;Vtg,V;G1,dB;G2,dB;HG,dB;VG,dB;Rot,dg;Cap;Input;Switch;BalGain,t;BalPh,dg;DeltaX;DeltaY;Filter;N;L;LF;HF;II_LF;II_HF;ASD;R;BR;Width;Height;Alfa;Beta;X0;Y0;X1;Y1;Dir;Sim;Pol").split(";"));
    ui->tableWidget_channels->selectRow(pNetDevice->getSysSets()->cur_freq);

    for(int chan=0;chan<max_freq_num;chan++)
    {
        pChan=pNetDevice->getChanSets(chan);
        pAsdFrame=&pNetDevice->getChanASDSets(chan)->Frame;
        pFilter=pNetDevice->getChanFilterSets(chan);
        val=0;
        c=0;
        ui->tableWidget_channels->setItem(chan,val, new QTableWidgetItem);
        item=ui->tableWidget_channels->item(chan,val++);
        item->setBackground(colorsArray[chan&7]);
        item->setFlags(item->flags()^Qt::ItemIsSelectable);

        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(QString::number(pChan->frequency_hz/1000)));ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(QString::number(pChan->voltage_v)));      ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(QString::number(pChan->gain1_db)));       ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(QString::number(pChan->gain2_db)));       ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(QString::number(pChan->h_gain_db)));      ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(QString::number(pChan->v_gain_db)));      ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(QString::number(pChan->rotation)));       ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(QString::number(pChan->capacitor)));      ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(ui->comboBox_chanInput->itemText(pChan->input_type)));ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(QString::number(pChan->ext_switch)));     ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
c++;
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(QString::number(pChan->balanceGain_t)));  ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(QString::number(pChan->balancePhase_dg)));ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(QString::number(pChan->Ku2DeltaX)));      ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(QString::number(pChan->Ku2DeltaY)));      ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
c++;
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(ui->comboBox_filterType->itemText(pFilter->type)));   ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(QString::number(pFilter->averValue)));                ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(QString::number(pFilter->difValue)));                 ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(QString::number(pFilter->lfValue)));                  ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(QString::number(pFilter->hfValue)));                  ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(QString::number(pFilter->iir_lfValue)));              ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(QString::number(pFilter->iir_hfValue)));              ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
c++;
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(ui->comboBox_asdType->itemText(pAsdFrame->Type)));    ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(QString::number(pAsdFrame->Radius)));                 ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(QString::number(pAsdFrame->BRadius)));                ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(QString::number(pAsdFrame->Width)));                  ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(QString::number(pAsdFrame->Height)));                 ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(QString::number(pAsdFrame->Alfa)));                   ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(QString::number(pAsdFrame->Beta)));                   ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(QString::number(pAsdFrame->pX[0])));                  ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(QString::number(pAsdFrame->pY[0])));                  ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(QString::number(pAsdFrame->pX[1])));                  ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(QString::number(pAsdFrame->pY[1])));                  ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(ui->comboBox_asdDirection->itemText(pAsdFrame->Direction)));      ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(ui->comboBox_asdIsSimetric->itemText(pAsdFrame->isSimetric)));    ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
        ui->tableWidget_channels->setItem(chan,val,new QTableWidgetItem(ui->comboBox_asdPolarity->itemText(pAsdFrame->Polarity)));        ui->tableWidget_channels->item(chan,val++)->setBackground(colors[c]);
    }
}



void MainWindow::fillChan(int _chan)
{
    TNetDevice* pNetDevice=client->getNetDevice();
    pNetDevice->fillChannelSets(_chan,
                                ui->spinBox_chanFreq->value()*1000,
                                ui->comboBox_chanVoltage->currentText().toFloat(),
                                ui->doubleSpinBox_chanPreGain->value(),
                                ui->doubleSpinBox_chanGain->value(),
                                ui->doubleSpinBox_chanHGain->value(),
                                ui->doubleSpinBox_chanVGain->value(),
                                ui->doubleSpinBox_chanRotation->value(),
                                ui->spinBox_chanCapacitor->value(),
                                ui->comboBox_chanInput->currentIndex(),
                                ui->spinBox_chanSwitcher->value(),
                                ui->spinBox_chanBalGain->value(),
                                ui->doubleSpinBox_chanBalPh->value(),
                                ui->spinBox_chanKu2DeltaX->value(),
                                ui->spinBox_chanKu2DeltaY->value()
                                );
    pNetDevice->fillChannelASDSets(_chan,1,
                                   ui->comboBox_asdType->currentIndex(),
                                   ui->spinBox_asdRadius->value(),
                                   ui->spinBox_asdBRadius->value(),
                                   ui->spinBox_asdWidth->value(),
                                   ui->spinBox_asdHeight->value(),
                                   ui->spinBox_asdAlfa->value(),
                                   ui->spinBox_asdBeta->value(),
                                   ui->comboBox_asdIsSimetric->currentIndex(),
                                   ui->comboBox_asdPolarity->currentIndex(),
                                   ui->spinBox_asdX0->value(),
                                   ui->spinBox_asdY0->value(),
                                   ui->spinBox_asdX1->value(),
                                   ui->spinBox_asdY1->value(),
                                   ui->comboBox_asdDirection->currentIndex()
                                   );
    pNetDevice->fillChannelFilterSets(_chan,
                                      ui->comboBox_filterType->currentIndex(),
                                      ui->spinBox_filterAverValue->value(),
                                      ui->spinBox_filterDifValue->value(),
                                      ui->spinBox_filterLFValue->value(),
                                      ui->spinBox_filterHFValue->value(),
                                      ui->spinBox_filterIIR_LFValue->value(),
                                      ui->spinBox_filterIIR_HFValue->value()
                                      );
}
void MainWindow::fillSets()
{
    HWNetSysSets* pSysSets=client->getNetDevice()->getSysSets();
    HWNetMainSets* pMainSets=client->getNetDevice()->getMainSets();
    if(isChangeSignalActive)
    {
        pSysSets->active_freq_number=client->getNetDevice()->getChannelCount();//ui->spinBox_activeFreqNum->value();
        pSysSets->adc_meas_dly_t=ui->spinBox_adcMeasDly->value();
        pSysSets->corelation_deep=ui->spinBox_corelationDeep->value();
        pSysSets->corelation_scale=ui->spinBox_corelationScale->value();
        //pSysSets->cur_freq=ui->spinBox_curFreq->value();
        pSysSets->cur_freq=ui->spinBox_curChannel->value();
        pSysSets->meas_dly_t=ui->spinBox_measDly->value();
        pSysSets->meas_len_t=ui->spinBox_measLen->value();
        pSysSets->meas_period_num=ui->spinBox_measPeriodNum->value();

        pMainSets->input_switch=ui->comboBox_inputType->currentIndex();
        pMainSets->ext_switch_type=ui->comboBox_extSwitchType->currentIndex();
        pMainSets->is_double_freq_mode=ui->comboBox_isDoubleFreqMode->currentIndex();
        pMainSets->max_Fsmp_hz=ui->spinBox_maxFsmp->value();
        pMainSets->measOnMm=ui->spinBox_measOnmm->value();
        pMainSets->pulse_on_meas=ui->spinBox_pulseOnMeas->value();
        pMainSets->rvtpAutoDetectMode=ui->comboBox_rvtpAutodetect->currentIndex();
        pMainSets->rvtp_rounds_count=ui->spinBox_roundsCount->value();
        pMainSets->rvtp_speed_index=ui->comboBox_rvtpSpeed->currentIndex();
        pMainSets->sync_type=ui->comboBox_syncType->currentIndex();
        pMainSets->testMode=ui->comboBox_testMode->currentIndex();
    }
}
void MainWindow::showSets()
{
    HWNetSysSets* pSysSets=client->getNetDevice()->getSysSets();
    HWNetMainSets* pMainSets=client->getNetDevice()->getMainSets();
    isChangeSignalActive=false;
    //ui->spinBox_activeFreqNum->setValue(pSysSets->active_freq_number);
    ui->spinBox_adcMeasDly->setValue(pSysSets->adc_meas_dly_t);
    ui->spinBox_corelationDeep->setValue(pSysSets->corelation_deep);
    ui->spinBox_corelationScale->setValue(pSysSets->corelation_scale);
    //ui->spinBox_curFreq->setValue(pSysSets->cur_freq);
    ui->spinBox_measDly->setValue(pSysSets->meas_dly_t);
    ui->spinBox_measLen->setValue(pSysSets->meas_len_t);
    ui->spinBox_measPeriodNum->setValue(pSysSets->meas_period_num);

    ui->comboBox_inputType->setCurrentIndex(pMainSets->input_switch);
    ui->comboBox_extSwitchType->setCurrentIndex(pMainSets->ext_switch_type);
    ui->comboBox_isDoubleFreqMode->setCurrentIndex(pMainSets->is_double_freq_mode);
    ui->spinBox_maxFsmp->setValue(pMainSets->max_Fsmp_hz);
    ui->spinBox_measOnmm->setValue(pMainSets->measOnMm);
    ui->spinBox_pulseOnMeas->setValue(pMainSets->pulse_on_meas);
    ui->comboBox_rvtpAutodetect->setCurrentIndex(pMainSets->rvtpAutoDetectMode);
    ui->spinBox_roundsCount->setValue(pMainSets->rvtp_rounds_count);
    ui->comboBox_rvtpSpeed->setCurrentIndex(pMainSets->rvtp_speed_index);
    ui->comboBox_syncType->setCurrentIndex(pMainSets->sync_type);
    ui->comboBox_testMode->setCurrentIndex(pMainSets->testMode);
    isChangeSignalActive=true;
}

void MainWindow::on_pushButton_add_clicked()
{
    int chan;
    chan=client->getNetDevice()->addChannel();
    if(chan>=0)
    {
        fillChan(chan);
        showChannels();
        ui->spinBox_curChannel->setMaximum(client->getNetDevice()->getChannelCount()-1);
    }
}

void MainWindow::on_pushButton_del_clicked()
{
    if(client->getNetDevice()->delChannel())
    {
        showCurChannel();
        showChannels();
        ui->spinBox_curChannel->setMaximum(client->getNetDevice()->getChannelCount()-1);
    }
}
void MainWindow::showCurChannel()
{
    int chan= client->getNetDevice()->getCurChannel();
    HWNetChan*              pChan=client->getNetDevice()->getChanSets(chan);
    HWNetChanASD*           pASD=client->getNetDevice()->getChanASDSets(chan);
    HWNetChanFilter*        pFilter=client->getNetDevice()->getChanFilterSets(chan);
    //HWNetChanKalibrCurve*   pKalibrCurve=client->getNetDevice()->getChanKalibrCurveSets(chan);
    isChangeSignalActive=false;

    ui->spinBox_chanFreq->setValue(pChan->frequency_hz/1000);

    ui->comboBox_chanVoltage->setCurrentIndex(trunc(pChan->voltage_v));

    ui->doubleSpinBox_chanPreGain->setValue(pChan->gain1_db);
    ui->doubleSpinBox_chanGain->setValue(pChan->gain2_db);
    ui->doubleSpinBox_chanHGain->setValue(pChan->h_gain_db);
    ui->doubleSpinBox_chanVGain->setValue(pChan->v_gain_db);
    ui->doubleSpinBox_chanRotation->setValue(pChan->rotation);
    ui->spinBox_chanCapacitor->setValue(pChan->capacitor);
    ui->comboBox_chanInput->setCurrentIndex(pChan->input_type);
    ui->spinBox_chanSwitcher->setValue(pChan->ext_switch);
    ui->spinBox_chanBalGain->setValue(pChan->balanceGain_t);
    ui->doubleSpinBox_chanBalPh->setValue(pChan->balancePhase_dg);
    ui->spinBox_chanKu2DeltaX->setValue(pChan->Ku2DeltaX);
    ui->spinBox_chanKu2DeltaY->setValue(pChan->Ku2DeltaY);

    ui->comboBox_asdType->setCurrentIndex(pASD->Frame.Type);
    ui->spinBox_asdRadius->setValue(pASD->Frame.Radius);
    ui->spinBox_asdBRadius->setValue(pASD->Frame.BRadius);
    ui->spinBox_asdWidth->setValue(pASD->Frame.Width);
    ui->spinBox_asdHeight->setValue(pASD->Frame.Height);
    ui->spinBox_asdAlfa->setValue(pASD->Frame.Alfa);
    ui->spinBox_asdBeta->setValue(pASD->Frame.Beta);
    ui->comboBox_asdIsSimetric->setCurrentIndex(pASD->Frame.isSimetric);
    ui->comboBox_asdPolarity->setCurrentIndex(pASD->Frame.Polarity);
    ui->spinBox_asdX0->setValue(pASD->Frame.pX[0]);
    ui->spinBox_asdY0->setValue(pASD->Frame.pY[0]);
    ui->spinBox_asdX1->setValue(pASD->Frame.pX[1]);
    ui->spinBox_asdY1->setValue(pASD->Frame.pY[1]);
    ui->comboBox_asdDirection->setCurrentIndex(pASD->Frame.Direction);

    ui->comboBox_filterType->setCurrentIndex(pFilter->type);
    ui->spinBox_filterAverValue->setValue(pFilter->averValue);
    ui->spinBox_filterDifValue->setValue(pFilter->difValue);
    ui->spinBox_filterLFValue->setValue(pFilter->lfValue);
    ui->spinBox_filterHFValue->setValue(pFilter->hfValue);
    ui->spinBox_filterIIR_LFValue->setValue(pFilter->iir_lfValue);
    ui->spinBox_filterIIR_HFValue->setValue(pFilter->iir_hfValue);
    isChangeSignalActive=true;
}

void MainWindow::on_spinBox_curChannel_valueChanged(int arg1)
{
    if(isChangeSignalActive)
    {
        client->getNetDevice()->setCurChannel(arg1);
        ui->tableWidget_channels->selectRow(arg1);
        showCurChannel();
        qDebug()<<"on_spinBox_curChannel_valueChanged";
    }
}

void MainWindow::on_tableWidget_channels_cellClicked(int row, int column)
{
    isChangeSignalActive=false;
    cout<<row<<" "<<column<<endl;
    client->getNetDevice()->setCurChannel(row);
    ui->spinBox_curChannel->setValue(row);
    showCurChannel();
    isChangeSignalActive=true;
}

void MainWindow::on_checkBox_getData_stateChanged(int arg1)
{
    if(arg1)
        timer_getdata->start(ui->spinBox_get_data_interval->value());
    else
        timer_getdata->stop();
}

void MainWindow::on_checkBox_getTester_stateChanged(int arg1)
{
    if(arg1)
        timer_gettester->start(GET_TESTER_TIMER_PERIOD);
    else
        timer_gettester->stop();
}

void MainWindow::on_comboBox_swepWidth_currentIndexChanged(int index)
{
    setSweepRange(index);
    ui->customPlot_SWEEP->replot();
    ui->customPlot_BIT->replot();
    ui->customPlot_XY->replot();
}

void MainWindow::on_horizontalSlider_Scale_valueChanged(int value)
{
    setXYRange(value);
    ui->customPlot_XY->replot();
    ui->customPlot_SWEEP->replot();
    ui->customPlot_BIT->replot();
    ui->customPlot_RAW->replot();
}




void MainWindow::on_lineEdit_Bush_textChanged(const QString &arg1)
{
    client->getNetDevice()->setBashCmdLine(arg1);
}

void MainWindow::on_pushButton_RECONNECT_clicked()
{
    connectToEddycon();
}
void MainWindow::sendSTART()
{
    client->send_CMD_SRV_VERSION();
    sendALLsets();
    client->send_CMD_RUN_LOADING();
    client->send_CMD_RUN_CLEARING();
    client->send_CMD_RUN_START();
    client->getNetDevice()->clearDataBuffers();

    ui->checkBox_getData->setChecked(true);
    ui->checkBox_getTester->setChecked(true);
}

void MainWindow::sendALLsets()
{
    client->send_CMD_SET_MAINSETS();
    client->send_CMD_SET_SYSSETS();
    for(int chan=0;chan<client->getNetDevice()->getChannelCount();chan++)
    {
        client->send_CMD_SET_CHANNEL_SETS(chan,1);
        client->send_CMD_SET_CHANNEL_ASD(chan);
        client->send_CMD_SET_CHANNEL_FILTER(chan);
    }
}

void MainWindow::on_pushButton_START_XY_clicked()
{
    ui->comboBox_DataType->setCurrentIndex(0);
    ui->comboBox_testMode->setCurrentIndex(9);
    sendSTART();
}
void MainWindow::on_pushButton_START_RAW_clicked()
{
    ui->comboBox_DataType->setCurrentIndex(1);
    ui->comboBox_testMode->setCurrentIndex(3);
    sendSTART();
}
void MainWindow::on_pushButton_STOP_clicked()
{
    ui->checkBox_getData->setChecked(false);
    ui->checkBox_getTester->setChecked(false);
    client->send_CMD_RUN_STOP();
}

void MainWindow::on_pushButton_SET_ALL_clicked()
{
    sendALLsets();
    client->send_CMD_RUN_LOADING();
}

void MainWindow::LoadSettingsAction()
{
    TNetDevice* pdevice=client->getNetDevice();
    QString filename = QFileDialog::getOpenFileName( this, tr("Save settings"), QDir::currentPath()+"\\settings", tr("Settings files (*.stg);;All files(*.*)"), 0, QFileDialog::DontUseNativeDialog );
    QFile file;
    if( !filename.isNull() )
    {
        file.setFileName(filename);
        qDebug()<<"Try to open file:"<<filename;
        file.open(QIODevice::ReadOnly);
        file.read((char*)pdevice->getSysSets(),pdevice->getSysSetsSize());
        file.read((char*)pdevice->getMainSets(),pdevice->getMainSetsSize());
        file.read((char*)pdevice->getBalanseSets(),pdevice->getBalanseSetSize());
        file.read((char*)pdevice->getMeasCursorSets(),pdevice->getMeasCursorSetsSize());
        for(int mix=0;mix<pdevice->getMaxMixNumber();mix++)
            file.read((char*)pdevice->getMixSets(mix),pdevice->getMixSetsSize());
        for(int chan=0;chan<pdevice->getMaxFreqNumber();chan++)
        {
            file.read((char*)pdevice->getChanSets(chan),pdevice->getChanSetsSize());
            file.read((char*)pdevice->getChanASDSets(chan),pdevice->getChanASDSetsSize());
            file.read((char*)pdevice->getChanFilterSets(chan),pdevice->getChanFilterSetsSize());
            file.read((char*)pdevice->getChanKalibrCurveSets(chan),pdevice->getChanKalibrCurveSetsSize());
        }
        file.close();

        pdevice->calcChannels();
        showChannels();
        showCurChannel();
        showSets();
        ui->pushButton_STOP->clicked();
        ui->spinBox_curChannel->setMaximum(client->getNetDevice()->getChannelCount()-1);
    }
}
void MainWindow::SaveSettingsAction()
{
    TNetDevice* pdevice=client->getNetDevice();
    QString filename = QFileDialog::getSaveFileName( this, tr("Save settings"), QDir::currentPath()+"\\settings", tr("Settings files (*.stg);;All files(*.*)"), 0, QFileDialog::DontUseNativeDialog );
    QFile file;
    if( !filename.isNull() )
    {
        if(!filename.endsWith(".stg"))
            filename.append(".stg");
        file.setFileName(filename);
        qDebug()<<"Try to save file:"<<filename;
        file.open(QIODevice::WriteOnly);
        file.write((const char*)pdevice->getSysSets(),pdevice->getSysSetsSize());
        file.write((const char*)pdevice->getMainSets(),pdevice->getMainSetsSize());
        file.write((const char*)pdevice->getBalanseSets(),pdevice->getBalanseSetSize());
        file.write((const char*)pdevice->getMeasCursorSets(),pdevice->getMeasCursorSetsSize());
        for(int mix=0;mix<pdevice->getMaxMixNumber();mix++)
            file.write((const char*)pdevice->getMixSets(mix),pdevice->getMixSetsSize());
        for(int chan=0;chan<pdevice->getMaxFreqNumber();chan++)
        {
            file.write((const char*)pdevice->getChanSets(chan),pdevice->getChanSetsSize());
            file.write((const char*)pdevice->getChanASDSets(chan),pdevice->getChanASDSetsSize());
            file.write((const char*)pdevice->getChanFilterSets(chan),pdevice->getChanFilterSetsSize());
            file.write((const char*)pdevice->getChanKalibrCurveSets(chan),pdevice->getChanKalibrCurveSetsSize());
        }

        file.close();
    }
}


void MainWindow::on_comboBox_DataType_currentIndexChanged(int index)
{
    setXYRange(ui->horizontalSlider_Scale->value());
    ui->customPlot_RAW->replot();
}
/*
void MainWindow::on_comboBox_viewType_currentIndexChanged(int index)
{
    setViewType(index);
    setPlotsRange(ui->horizontalSlider_Scale->value(),ui->comboBox_swepWidth->currentIndex());
}

void MainWindow::on_comboBox_multyType_currentIndexChanged(int index)
{
    setMultyType(index);
}
*/
void MainWindow::on_customPlot_XY_mousePressed(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        client->getNetDevice()->changeViewType();
        setPlotsRange(ui->horizontalSlider_Scale->value(),ui->comboBox_swepWidth->currentIndex());
    }
    else
        client->getNetDevice()->changeMultyType();
}



void MainWindow::on_spinBox_get_data_interval_valueChanged(int arg1)
{
    timer_getdata->setInterval(arg1);
}
