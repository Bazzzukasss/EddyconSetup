#ifndef THREADS_H
#define THREADS_H

#include <QThread>
#include <QDebug>
#include "UDPClient_EddyconC.h"
#define THREAD_SLEEP_TIME_ms   100
typedef void (*Thread_proc)();

class TThread:public QThread
{
    Q_OBJECT
public:
    TThread(Thread_proc _proc,unsigned long _sleep_time_ms=THREAD_SLEEP_TIME_ms)
    {
        proc=_proc;
        sleep_time_ms=_sleep_time_ms;
    }

    void run()
    {
        while(true)
        {
            proc();
            msleep(sleep_time_ms);
        }
    }

private:
    Thread_proc proc;
    unsigned long sleep_time_ms;
};


class TNetClientThread:public QThread
{
    Q_OBJECT
public:
    TNetClientThread(UDPClient_EddyconC* _pClient,unsigned long _sleep_time_ms=THREAD_SLEEP_TIME_ms)
    {
        pClient=_pClient;
        sleep_time_ms=_sleep_time_ms;
    }

    void run()
    {
        while(true)
        {
            if(pClient->getNetDevice()->prepareDrawData())
                emit ProcedureFinished();
            msleep(sleep_time_ms);
        }
    }
signals:
    void ProcedureFinished();
private:
    UDPClient_EddyconC* pClient;
    unsigned long sleep_time_ms;
    int procId;
};
#endif // THREADS_H
