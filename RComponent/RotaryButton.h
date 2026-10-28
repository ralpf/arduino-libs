#pragma once

#include "Button.h"


class RotaryButton : public Button
{
    protected:
    ui8     pinDT;
    ui8     pinCLK;
    ui8     rotState = 0;
    i8      rotSteps = 0;
    bool    phaseCW  = false;
    bool    phaseCCW = false;
    bool    isInverted = false;
    //..........CTOR
    public:
    RotaryButton() = delete;
    RotaryButton(ui8 pinBT, ui8 pinDT, ui8 pinCLK, bool isPullup);
    //..........MEHTOD
    public:
    void Info() override;
    bool Update() override;
    bool IsClockWise();
    bool IsCounterClockWise();
    void InvertRotation(bool invert);
    private:
    ui8 ReadRotaryState();
};
