#pragma once

#include "Component.h"


class PirSensor : public ComponentBase
{
    //..........FIELD
    private:
    bool logicalOn;
    ui8  pin;
    bool isWarmupDone;                 // warm up flag
    ui8  warmupTime;                   // warm up time in seconds (self calibration)
                                       // typical takes 20 to 60 seconds

    //..........CTOR
    public:
    PirSensor() = delete;
    PirSensor(ui8 pin);
    PirSensor(ui8 pin, ui8 warmupSeconds);

    //..........METHOD
    public:
    bool Update() override;
    void Info()   override;
    bool IsMovement();                  // true if sensor detected movement
};
