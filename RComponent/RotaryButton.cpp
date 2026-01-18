#include "RotaryButton.h"

#include <uniPrinter.h>
#include <Arduino.h>


RotaryButton::RotaryButton(ui8 pinBT, ui8 pinDT, ui8 pinCLK, bool isPullup) : Button(pinBT, isPullup, 0)
{
    // 0 - no debounce
    this->pinDT  = pinDT;
    this->pinCLK = pinCLK;
    pinMode(pinDT,  isPullup ? INPUT_PULLUP : INPUT);
    pinMode(pinCLK, isPullup ? INPUT_PULLUP : INPUT);
}


bool RotaryButton::Update()
{
    bool changed = Button::Update();

    this->phaseCW  = false;
    this->phaseCCW = false;

    ui8 currState = this->ReadRotaryState();
    if (this->rotState != currState)
    {
        if (rotState != 0 && currState == 0)        // finishing rotation
        {
            if (rotState == 1) phaseCW  = true;
            if (rotState == 2) phaseCCW = true;
        }
        this->rotState = currState;
    }
    return changed | this->phaseCW | this->phaseCCW;
}


bool RotaryButton::IsClockWise()
{
    return phaseCW;
}


bool RotaryButton::IsCounterClockWise()
{
    return phaseCCW;
}


void RotaryButton::Info()
{
    SPrint("i: rotary button, pins: bt=%d dt=%d clk=%d isPullup=%d", pin, pinDT, pinCLK, isPullup);
}


ui8 RotaryButton::ReadRotaryState()
{
    bool dt  = digitalRead(this->pinDT)  == (this->isPullup ? 0 : 1);
    bool clk = digitalRead(this->pinCLK) == (this->isPullup ? 0 : 1);
    return (dt << 1) | clk;     // merge 2 bits in one byte
}