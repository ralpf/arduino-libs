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
    this->phaseDown = false;
    this->phaseUp   = false;

    int state = digitalRead(this->pin);
    bool physicalDown = state == (isPullup ? 0 : 1);

    if (physicalDown != logicalDown)
    {
        // here we are once per phase transition
        logicalDown = physicalDown;
        if (this->debounce)     // cast from int
        {
            SPrint("e: debounce not implemented");
            // this can return early
        }

        if (logicalDown) phaseDown = true;
        else             phaseUp   = true;
    }
    return this->phaseDown | this->phaseUp;
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