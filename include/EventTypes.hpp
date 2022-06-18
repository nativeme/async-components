#ifndef _EVENTTYPES_HPP_
#define _EVENTTYPES_HPP_

#include "Arduino.h"
#include "Behavable.hpp"

class IEvent{
public:
    IEvent(){}
    virtual ~IEvent(){}
    virtual void execute() = 0;
};

class SingleEvent
:   public IEvent
{
private:    
    std::function<void(void)> event_function;
public:
    SingleEvent(std::function<void(void)>&& event_function = [](){})
    :   IEvent(),
        event_function(event_function)
    {}
    virtual ~SingleEvent(){}

    void execute() override {
        this->event_function.operator()();
    }
};

#endif
