#include "Button.h"

#include <uniPrinter.h>
#include <Arduino.h>


#define DEFAULT_DEBOUNCE 30

//..................................CTOR


Button::Button(ui8 pin, bool isPullup) : Button(pin, isPullup, DEFAULT_DEBOUNCE)
{
}


Button::Button(ui8 pin, bool isPullup, ui16 debounce)
{
    this->pin = pin;
    this->isPullup = isPullup;
    this->debounce = debounce;
    pinMode(pin, isPullup ? INPUT_PULLUP : INPUT);
}

//..................................METHOD

bool Button::Update()
{
    phaseDown = false;
    phaseUp   = false;

    bool state = digitalRead(pin) == (isPullup ? 0 : 1);
    ui32 now = millis();

    if (state != physicalDown)
    {
        physicalDown = state;
        changedTime = now;
    }

    if (physicalDown == logicalDown)  return false;
    if (now - changedTime < debounce) return false;

    logicalDown = physicalDown;
    if (logicalDown) phaseDown = true;
    else             phaseUp   = true;

    return true;
}


bool Button::IsDown()
{
    return this->phaseDown;
}


bool Button::IsClick()
{
    return this->phaseUp;
}


bool Button::IsHold()
{
    return this->logicalDown;
}


void Button::Info()
{
    SPrint("i: button pin=%d isPullup=%d debounce=%d", pin, isPullup, debounce);
}