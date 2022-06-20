#include "Encoder.hpp"

namespace async{

const uint8_t Encoder::read_encoder() const {
    uint8_t val = 0;
    if(vpin_a->digital_read()) 
        val |= (1 << 1);
    if(vpin_b->digital_read())  
        val |= (1 << 0);
    return val;
}

Encoder::Encoder(const VPin& vpin_a,
        const VPin& vpin_b,
        const String& name,
        Params&& builder)
:   ILoopable(),
    Named(name),
    Behavable(2),
    vpin_a(std::make_unique<VPin>(vpin_a)),
    vpin_b(std::make_unique<VPin>(vpin_b))
{
    builder();
    Runtime::loopables.push_back(this);
};

Encoder::Encoder(const uint8_t& pin_a,
        const uint8_t& pin_b,
        const String& name,
        Params&& builder)
:   ILoopable(),
    Named(name),
    Behavable(2),
    vpin_a(new VPin(pin_a)),
    vpin_b(new VPin(pin_b))
{
    builder();
    Runtime::loopables.push_back(this);
};

Encoder::~Encoder(){
};

void Encoder::set_on_left(std::function<void(void)>&& on_left_event){
    auto ev = new SingleEvent(std::move(on_left_event));
    this->get_active_behavior().events_vec[on_left] = ev;
}

void Encoder::set_on_right(std::function<void(void)>&& on_right_event){
    auto ev = new SingleEvent(std::move(on_right_event));
    this->get_active_behavior().events_vec[on_right] = ev;
}

void Encoder::loop(){
    enc_new_val = read_encoder();
    if     ((enc_val == 3 && enc_new_val == 1) || (enc_val == 0 && enc_new_val == 2))
        this->get_active_behavior().events_vec[on_left]->execute();
    else if((enc_val == 2 && enc_new_val == 0) || (enc_val == 1 && enc_new_val == 3))
        this->get_active_behavior().events_vec[on_right]->execute();
    enc_val = enc_new_val;
}

}
