#include "timer.h"

Timer::Timer(uint intervals, const function<void(void)> &fun)
{
    Intervals=intervals;
    Fun=fun;
    Reset();
}

void Timer::Reset()
{
    BeginTime=QDateTime::currentMSecsSinceEpoch();
    EndTime=BeginTime+Intervals;
    IsComplete=0;
}

void Timer::start()
{
    if(IsComplete)return;
    if(QDateTime::currentMSecsSinceEpoch()>=EndTime){
        IsComplete=1;
        Fun();
    }
}
