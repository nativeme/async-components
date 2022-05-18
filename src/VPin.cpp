#include "VPin.hpp"

VPin::VPin(std::function<void(uint8_t)>&& digital_write,
           std::function<bool(void)>&&    digital_read,
           std::function<void(uint8_t)>&& pin_mode)
:   digital_write(digital_write),
    digital_read(digital_read),
    pin_mode(pin_mode){};

VPin::~VPin(){};

void VPin::digitalWrite(const uint8_t& state){
    this->digital_write(state);
}

bool VPin::digitalRead() const {
    return this->digital_read();
}

void VPin::pinMode(const uint8_t& mode){
    this->pin_mode(mode);
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
    this->digital_write(high);
}

void VPin::off(){
    this->digital_write(low);
}

void VPin::toggle(){
    this->digital_write(!digital_read());
}