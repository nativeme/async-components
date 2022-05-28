#include "Button.hpp"
namespace async{

    Button::Button(const VPin& vpin,
           const String& name = "",
           std::vector<std::function<void(void)>>&& standard_behavior = {})
    :   Named(name),
        Behavable(),
        vpin(std::make_unique<VPin>(vpin))
    {   
        this->vpin->pin_mode(INPUT_PULLUP);
        /*
            If there are less functions than required to create behavior for this item,
            fill lacking functions with dummy lambdas.
        */
        if(standard_behavior.size() < 3){
            uint8_t original_size = standard_behavior.size();
            standard_behavior.resize(3);
            for (size_t i = original_size; i < 3; i++){
                standard_behavior[i] = [](){};
            }
        }

        this->new_behavior(std::move(standard_behavior));
        Runtime::loopables.push_back(this);
    };

    Button::Button(const uint8_t pin, 
           const String& name = "", 
           std::vector<std::function<void(void)>>&& standard_behavior = {})
    :   Named(name),
        Behavable(),   
        vpin(new VPin(pin))
    {
        this->vpin->pin_mode(INPUT_PULLUP);
 
        if(standard_behavior.size() < 3){
            uint8_t original_size = standard_behavior.size();
            standard_behavior.resize(3);
            for (size_t i = original_size; i < 3; i++){
                standard_behavior[i] = [](){};
            }
        }

        this->new_behavior(std::move(standard_behavior));
        Runtime::loopables.push_back(this);
    };

    Button::~Button(){
    };

    void Button::set_on_press(const std::function<void(void)>&& on_press_event){
        this->behaviors[active_behavior_id].event_set[on_press] = on_press_event;
    }

    void Button::set_on_release(const std::function<void(void)>&& on_release_event){
        this->behaviors[active_behavior_id].event_set[on_release] = on_release_event;
    }

    void Button::set_on_hold_loop(std::function<void(void)>&& on_hold_codeblock){
        this->behaviors[active_behavior_id].event_set[on_hold] = on_hold_codeblock;
    }

    void Button::set_on_hold_after(const uint32_t press_duration, std::function<void(void)>&& on_hold_event){
        this->event_timer.add_event(press_duration, std::move(on_hold_event));
    }

    uint32_t Button::get_current_press_duration() const {
        return event_timer.get_time_passed();
    }

    const uint32_t& Button::get_last_press_time(){
        return last_press_time;
    }

    void Button::loop(){
        switch (state){
        case Button::idle:
            saved_bounce_logic_state = vpin->digital_read();
            if(saved_bounce_logic_state){
                debounce_timer.start();
                state = Button::pre_bounce_testing;
            }
            break;
        case Button::pre_bounce_testing:
            if(debounce_timer.have_passed(20)){
                if(vpin->digital_read() == saved_bounce_logic_state){
                    event_timer.start();
                    this->behaviors[active_behavior_id].event_set[on_press]();
                    state = Button::pressed;
                } else
                    state = Button::idle;
            }
            break;
        case Button::pressed:
            if(vpin->digital_read() == !saved_bounce_logic_state){
                state = Button::post_bounce_testing;
                debounce_timer.start();
            } else
                this->behaviors[active_behavior_id].event_set[on_hold]();
            break;
        case Button::post_bounce_testing:
            if(debounce_timer.have_passed(30)){
                if(vpin->digital_read() == !saved_bounce_logic_state){
                    last_press_time = event_timer.get_time_passed();
                    event_timer.clear();
                    this->behaviors[active_behavior_id].event_set[on_release]();
                    state = Button::idle;
                } else 
                    state = Button::pressed;
            }
            break;
        default:
            break;
        }
    }
};