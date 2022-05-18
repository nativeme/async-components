/**
 * @file VPWMPin.hpp
 * @author Łukasz Kaniak - nativeme (lukaszkaniak@gmail.com)
 * @brief 
 * Basic polymorphic wrapper around microcontroller's physical pin,
 * extended by software simulated PWM behavior.
 * @version 0.1
 * @date 2022-05-10
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef _VPWMPIN_HPP_
#define _VPWMPIN_HPP_

#include "Arduino.h"
#include "VPin.hpp"
#include "Timer.hpp"
#include "Runtime.hpp"
#include "ILoopable.hpp"

namespace async{
/**
 * @brief 
 * Basic polymorphic wrapper around microcontroller's physical pin,
 * extended by software simulated PWM behavior.
 */    
class VPWMPin
:   public ILoopable,
    public VPin
{
private:
    async::Timer pwm_width_timer = async::Timer(8500, true, true);
    async::Timer pwm_period_timer = async::Timer(17000, true, true);

    uint8_t state = true;

    enum State{
        off_state = 0,
        on_state = 1
    };

public:
    VPWMPin(){};
    VPWMPin(std::function<void(uint8_t)>&& digital_write, 
            std::function<bool(void)>&&    digital_read,
            std::function<void(uint8_t)>&& pin_mode);

    VPWMPin(const VPin& vpin);
    virtual ~VPWMPin() override;

    void set_width(const uint32_t &width);
    void set_period(const uint32_t &period);

    float get_duty_cycle();
    void set_duty_cycle(const float &duty_cycle);

    virtual void on() override;
    virtual void off() override;
    virtual void toggle() override;

    virtual void loop() override;
};
}
#endif
