#include "VPWMPin.hpp"
namespace async{

VPWMPin::VPWMPin(std::function<void(uint8_t)>&&  digital_write, 
                 std::function<bool(void)>&&     digital_read,
                 std::function<void(uint8_t)>&&  pin_mode)
:   VPin(std::move(digital_write), 
         std::move(digital_read), 
         std::move(pin_mode))
{
    pwm_width_timer.set_resolution(async::Timer::Resolution::micros_res);
    pwm_period_timer.set_resolution(async::Timer::Resolution::micros_res);        
    async::Runtime::loopables.push_back(this);
};

VPWMPin::VPWMPin(const VPin& vpin){
    auto casted_vpin = (const VPWMPin*)&vpin;
    this->digital_write = casted_vpin->digital_write;
    this->digital_read  = casted_vpin->digital_read;
    this->pin_mode      = casted_vpin->pin_mode;
    pwm_width_timer.set_resolution(async::Timer::Resolution::micros_res);
    pwm_period_timer.set_resolution(async::Timer::Resolution::micros_res);
    async::Runtime::loopables.push_back(this);
};

VPWMPin::~VPWMPin(){};

void VPWMPin::set_width(const uint32_t &width){
    pwm_width_timer.set_time(width);
};

void VPWMPin::set_period(const uint32_t &period){
    pwm_period_timer.set_time(period);
};

float VPWMPin::get_duty_cycle(){
    return (float)pwm_width_timer.get_base_time() / (float)pwm_period_timer.get_base_time() * 100;
};

void VPWMPin::set_duty_cycle(const float &duty_cycle){
    pwm_width_timer.set_time((float)pwm_period_timer.get_base_time() / 100 * duty_cycle);
};

void VPWMPin::on(){
    this->digital_write(high);
    state = VPWMPin::State::on_state;
};

void VPWMPin::off(){
    this->digital_write(low);
    state = VPWMPin::State::off_state;
};

void VPWMPin::toggle(){
    state = !state;
};

void VPWMPin::loop(){
    switch (this->state){
    case VPWMPin::State::off_state:
        break;
    case VPWMPin::State::on_state:
        if(pwm_period_timer.is_finished()){
            this->digitalWrite(high);
            pwm_width_timer.start();
        }    
        if(pwm_width_timer.is_finished()){
            this->digitalWrite(low);
            pwm_width_timer.clear();
        }
        break;
    default:
        break;
    }   
}

}
