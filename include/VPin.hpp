/**
 * @file VPin.hpp
 * @author Łukasz Kaniak - nativeme (lukaszkaniak@gmail.com)
 * @brief 
 * Basic polymorphic wrapper around microcontroller's physical pin.
 * @version 0.1
 * @date 2022-05-10
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef _VIRTUALPIN_HPP_
#define _VIRTUALPIN_HPP_
#include "Arduino.h"

/**
 * @brief 
 * Basic polymorphic wrapper around microcontroller's physical pin.
 */
class VPin
{
private:
protected:
    std::function<void(uint8_t)> digital_write;
    std::function<bool(void)>    digital_read;
    std::function<void(uint8_t)> pin_mode;
    bool high = 1;
    bool low = 0;
public:
    VPin(std::function<void(uint8_t)>&& digital_write = [](uint8_t state){}, 
         std::function<bool(void)>&&    digital_read  = [](){return false;},
         std::function<void(uint8_t)>&& pin_mode      = [](uint8_t mode){});
    virtual ~VPin();

    void digitalWrite(const uint8_t& state);
    bool digitalRead() const;
    void pinMode(const uint8_t& mode);

    void inverse_logic(bool inverse = true);
    virtual void on();
    virtual void off();
    virtual void toggle();
};

#endif
