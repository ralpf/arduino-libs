#include "timeService.h"

#include <Arduino.h>


TimeService timeSrv;


void TimeService::update()
{
    prevMs = nowMs;
    nowMs = millis();
    dtMs = nowMs - prevMs;
    ++frame;
    sec = nowMs / 1000.0f;
}