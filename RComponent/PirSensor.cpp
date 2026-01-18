#include "PirSensor.h"

#include <uniPrinter.h>
#include <Arduino.h>


#define  PIR_WARMUP_DEFAULT 30


PirSensor::PirSensor(ui8 pin) : PirSensor(pin, PIR_WARMUP_DEFAULT) { }


PirSensor::PirSensor(ui8 pin, ui8 warmupSeconds)
{
    this->pin = pin;
    this->warmupTime = warmupSeconds;
}


bool PirSensor::Update()
{
    if (!isWarmupDone)
    {
        // warmup assumes sensor is powerd up with arduino
        if (millis() < this->warmupTime * 1000) return false;
        else
        {
            this->isWarmupDone = true;
            SPrint("i: pir sensor warmup done (pin=%d)", this->pin);
        }
    }
    // here warmup is done
    bool somethingChanged = false;
    int physicalOn = digitalRead(this->pin);

    if (physicalOn != this->logicalOn)
    {
        this->logicalOn = physicalOn;
        somethingChanged = true;
    }
    return somethingChanged;
}


bool PirSensor::IsMovement()
{
    return this->logicalOn;
}


void PirSensor::Info()
{
    SPrint("i: pir sensor pin=%d warmup-time=%d", this->pin, this->warmupTime);
}