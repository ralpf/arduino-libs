#pragma once


class ComponentBase                     // Abstract Base
{
    public:
    virtual bool Update() = 0;          // abstarct, true if something changed
    virtual void Info();
};

