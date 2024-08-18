#ifndef TIMER_H
#define TIMER_H
#include"include.h"
#include"functional"
using std::function;
struct  Timer
{
    ULL BeginTime,EndTime,Intervals;
    function<void(void)>Fun;
    bool IsComplete;
    Timer(uint intervals,const function<void(void)>&fun);
    void Reset();
    void start();
};

#endif // TIMER_H
