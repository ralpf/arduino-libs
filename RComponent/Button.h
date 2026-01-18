#pragma once

#include "Types.h"
#include "ComponentBase.h"



class Button : public ComponentBase
{
    //..........FIELD
    protected:
    ui8     pin;
    ui16    debounce;
    bool    isPullup;
    bool    logicalDown;
    bool    phaseDown;
    bool    phaseUp;

    //..........CTOR
    public:
    Button() = delete;
    Button(ui8 pin, bool isPullup);
    Button(ui8 pin, bool isPullup, ui16 debounceMs);
    //..........MEHTOD
    public:
    bool Update() override;
    void Info()   override;
    bool IsDown();                          // once per phase transition (down)
    bool IsClick();                         // once per phase transition (up)
    bool IsHold();                          // while pressed
};
