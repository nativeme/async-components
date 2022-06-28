#include "AnalogBuzzer.hpp"

AnalogBuzzer::AnalogBuzzer(const uint8_t& vcc_pin, 
                const uint16_t& frequency)
:   vcc_pin(vcc_pin),
    frequency(frequency)
{
    pinMode(this->vcc_pin, OUTPUT);        
}

AnalogBuzzer::~AnalogBuzzer(){
}

void AnalogBuzzer::play(){
    analogWriteFreq(frequency);
    analogWrite(vcc_pin, 130);
    is_playing = true;
};

void AnalogBuzzer::play(const uint16_t& frequency){
    this->frequency = frequency;
    analogWriteFreq(frequency);
    analogWrite(vcc_pin, 130);
    is_playing = true;
};

void AnalogBuzzer::play_pitch(const String& pitch_name){
    this->frequency = pitch.at(pitch_name.c_str());
    analogWriteFreq(frequency);
    analogWrite(vcc_pin, 130);
    is_playing = true;
};

void AnalogBuzzer::stop(){
    if (is_playing == true){
        analogWrite(vcc_pin,0);
        digitalWrite(vcc_pin,0);
        is_playing = false;
    }
};

void AnalogBuzzer::toggle(){
    switch (this->is_playing){
    case false:
        play();
        break;
    default:
        stop();
        break;
    }
}

const uint16_t& AnalogBuzzer::get_frequency(){ 
    return this->frequency;
};

void AnalogBuzzer::set_frequency(const uint16_t& frequency){
    this->frequency = frequency; 
    analogWriteFreq(frequency);
};

