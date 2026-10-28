#pragma once

#include <Types.h>

class TimeService
{
    private:
    // all in ms
    ui32 nowMs  = 0;
    ui32 prevMs = 0;
    ui32 dtMs   = 0;
    ui32 frame  = 0;
    float sec   = 0;

    public:
    ui32 now()   const { return nowMs; }
    ui32 dt()    const { return dtMs; }
    float time() const { return sec; }     // seconds

    public:  // call this every loop
    void update();
};


extern TimeService timeSrv;