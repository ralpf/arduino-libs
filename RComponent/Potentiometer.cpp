#include "Potentiometer.h"
#include <uniPrinter.h>

#include <Arduino.h>

// note: it operates on range [0, 1024)
#define DEFAULT_THRESHOLD 5


Potentiometer::Potentiometer(ui8 pin) : Potentiometer(pin, DEFAULT_THRESHOLD)
{
}


Potentiometer::Potentiometer(ui8 pin, ui8 threshold)
{
    this->pin = pin;
    this->threshold = threshold;
    pinMode(pin, INPUT);
    // init value
    this->value = analogRead(pin);
}


ui16 Potentiometer::Value()
{
    return this->value;
}


ui8 Potentiometer::Value8()
{
    return map(this->value, 0, 1023, 0, 255);
}


bool Potentiometer::Update()
{
    ui16 current = analogRead(this->pin);
    if ( abs(current - this->value) > this->threshold)
    {
        this->value = current;
        return true;
    }
    return false;
}


void Potentiometer::Info()
{
    SPrint("i: potentiometer pin=%d threshold=%d", pin, threshold);
}