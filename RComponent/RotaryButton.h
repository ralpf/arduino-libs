#pragma once

#include "Button.h"


class RotaryButton : public Button
{
    protected:
    ui8     pinDT;
    ui8     pinCLK;
    ui8     rotState;
    bool    phaseCW;
    bool    phaseCCW;
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
    private:
    ui8 ReadRotaryState();
};
