#include "RotaryButton.h"

#include <uniPrinter.h>
#include <Arduino.h>




// encoder transition matrix
// Rows: previous state; Columns: current state.
static const i8 rotaryTransitions[4][4] = {
    //  0   1   2   3
    {   0, -1,  1,  0 },     // 0
    {   1,  0,  0, -1 },     // 1
    {  -1,  0,  0,  1 },     // 2
    {   0,  1, -1,  0 }      // 3
};


RotaryButton::RotaryButton(ui8 pinBT, ui8 pinDT, ui8 pinCLK, bool isPullup) : Button(pinBT, isPullup)
{
    this->pinDT  = pinDT;
    this->pinCLK = pinCLK;
    pinMode(pinDT,  isPullup ? INPUT_PULLUP : INPUT);
    pinMode(pinCLK, isPullup ? INPUT_PULLUP : INPUT);
    rotState = ReadRotaryState();
}


bool RotaryButton::Update()
{
    bool changed = Button::Update();

    phaseCW  = false;
    phaseCCW = false;

    ui8 currState = ReadRotaryState();
    if (rotState != currState)
    {
        if ((rotState ^ currState) == 3)  // both bits changed: missed or invalid transition
            rotSteps = 0;
        else
            rotSteps += rotaryTransitions[rotState][currState];

        rotState = currState;

        if (currState == 0)
        {
            if (rotSteps ==  4) phaseCW  = true;
            if (rotSteps == -4) phaseCCW = true;
            rotSteps = 0;
        }
    }
    return changed | phaseCW | phaseCCW;
}


bool RotaryButton::IsClockWise()
{
    return !isInverted ? phaseCW : phaseCCW;
}


bool RotaryButton::IsCounterClockWise()
{
    return !isInverted ? phaseCCW : phaseCW;
}


void RotaryButton::Info()
{
    SPrint("i: rotary button, pins: bt=%d dt=%d clk=%d isPullup=%d", pin, pinDT, pinCLK, isPullup);
}


void RotaryButton::InvertRotation(bool invert)
{
    isInverted = invert;
}


ui8 RotaryButton::ReadRotaryState()
{
    bool dt  = digitalRead(pinDT)  == (isPullup ? 0 : 1);
    bool clk = digitalRead(pinCLK) == (isPullup ? 0 : 1);
    return (dt << 1) | clk;     // merge 2 bits in one byte
}