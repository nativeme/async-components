#include "VPin.hpp"

VPin::VPin(std::function<bool(void)>&&    digital_read,
           std::function<void(uint8_t)>&& digital_write,
           std::function<void(uint8_t)>&& pin_mode)
:   digital_read_impl(digital_read),
    digital_write_impl(digital_write),
    pin_mode_impl(pin_mode){};

VPin::VPin(const uint8_t& physical_pin)
:   digital_read_impl(
    [&physical_pin](){
        return digitalRead(physical_pin);
    }),
    digital_write_impl(
    [&physical_pin](uint8_t state){
        digitalWrite(physical_pin, state);
    }),
    pin_mode_impl(
    [&physical_pin](uint8_t mode){
        pinMode(physical_pin, mode);
    })
{};

VPin::~VPin(){};

void VPin::digital_write(const uint8_t& state) const {
    this->digital_write_impl(state);
}

bool VPin::digital_read() const {
    switch (high){
    case 1:
        return this->digital_read_impl();
        break;
    default:
        return !this->digital_read_impl();
        break;
    }
}

void VPin::pin_mode(const uint8_t& mode) const {
    this->pin_mode_impl(mode);
}

void VPin::inverse_logic(bool inverse){
    if(inverse){
        high = 0;
        low = 1;
    }else{
        high = 1;
        low = 0;
    }
}

void VPin::on(){
    this->digital_write_impl(this->high);
}

void VPin::off(){
    this->digital_write_impl(this->low);
}

void VPin::toggle(){
    this->digital_write_impl(!digital_read_impl());
}