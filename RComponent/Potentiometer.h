#pragma once

#include "ComponentBase.h"
#include "Types.h"


class Potentiometer : public ComponentBase
{
    //..........FIELD
    protected:
    ui8     pin;
    ui8     threshold;                  // Threshold value for detecting and registering changes
    ui16    value;

    //..........CTOR
    public:
    Potentiometer() = delete;
    Potentiometer(ui8 pin);
    Potentiometer(ui8 pin, ui8 threshold);

    //..........MEHTOD
    public:
    bool Update() override;
    void Info()   override;
    ui16 Value();
    ui8  Value8();

};
