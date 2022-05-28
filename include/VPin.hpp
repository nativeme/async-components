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
    std::function<bool(void)>    digital_read_impl;
    std::function<void(uint8_t)> digital_write_impl;
    std::function<void(uint8_t)> pin_mode_impl;
    bool high = 1;
    bool low = 0;
public:
    VPin(std::function<bool(void)>&&    digital_read  = [](){return false;},
         std::function<void(uint8_t)>&& digital_write = [](uint8_t state){}, 
         std::function<void(uint8_t)>&& pin_mode      = [](uint8_t mode){});
    
    VPin(const uint8_t& physical_pin);
    
    virtual ~VPin();

    void digital_write(const uint8_t& state) const;
    bool digital_read() const;
    void pin_mode(const uint8_t& mode) const;

    void inverse_logic(bool inverse = true);
    virtual void on();
    virtual void off();
    virtual void toggle();
};

#endif
