#include "Button.hpp"
namespace async{

    Button::Button(const VPin& vpin,
           const String& name,
           std::vector<std::function<void(void)>*>&& standard_behavior)
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
                standard_behavior[i] = new std::function<void(void)>([](){});
            }
        }

        this->new_behavior(standard_behavior);
        Runtime::loopables.push_back(this);
    };

    // Button::Button(const uint8_t pin, 
    //        const String& name, 
    //        std::vector<std::function<void(void)>>&& standard_behavior)
    // :   Named(name),
    //     Behavable(),   
    //     vpin(new VPin(pin))
    // {
    //     this->vpin->pin_mode(INPUT_PULLUP);
 
    //     if(standard_behavior.size() < 3){
    //         uint8_t original_size = standard_behavior.size();
    //         standard_behavior.resize(3);
    //         for (size_t i = original_size; i < 3; i++){
    //             standard_behavior[i] = [](){};
    //         }
    //     }

    //     this->new_behavior(std::move(standard_behavior));
    //     Runtime::loopables.push_back(this);
    // };

    Button::~Button(){
    };

    void Button::set_on_press(std::function<void(void)>& on_press_event){
        this->behaviors[active_behavior_id]->event_set[on_press] = &on_press_event;
    }

    void Button::set_on_release(std::function<void(void)>&& on_release_event){
        this->behaviors[active_behavior_id]->event_set[on_release] = new std::function<void(void)>(on_release_event);
    }

    void Button::set_on_hold_loop(std::function<void(void)>& on_hold_codeblock){
        this->behaviors[active_behavior_id]->event_set[on_hold] = &on_hold_codeblock;
    }

    void Button::set_on_hold_after(const uint32_t& press_duration, std::function<void(void)>&& on_hold_event){
        this->behaviors[active_behavior_id]->event_set.push_back(new std::function<void(void)>(on_hold_event));
        longpress_events.push_back(LongPressEvent(
            press_duration,
            this->behaviors[active_behavior_id]->event_set.back()
        ));
    }

    void Button::set_on_multiclicks(const uint8_t& click_target, std::function<void(void)>&& multiclick_event){
        this->behaviors[active_behavior_id]->event_set.push_back(new std::function<void(void)>(multiclick_event));
        multiclick_events.push_back(MultiClickEvent(
            click_target, 
            this->behaviors[active_behavior_id]->event_set.back()
        ));
    }

    void Button::set_multiclick_interspace(const uint8_t& multiclick_interspace){
        this->multiclick_interspace = multiclick_interspace;
    }

    uint32_t Button::get_current_press_duration() const {
        return longpress_timer.get_time_passed();
    }

    const uint32_t& Button::get_last_press_time(){
        return last_press_time;
    }

    void Button::loop(){
        switch (state){
        case Button::idle:
            for (auto &&event : multiclick_events){
                if(!event.done && 
                    event.click_target == this->multiclick_count + 1 && 
                    multiclick_timer.get_time_passed() > multiclick_interspace){
                        event.event->operator()();
                        event.done = true;
                    }
            }

            saved_bounce_logic_state = vpin->digital_read();
            if(saved_bounce_logic_state){
                debounce_timer.start();
                state = Button::pre_bounce_testing;
            }

            break;
        case Button::pre_bounce_testing:
            if(debounce_timer.have_passed(20)){
                if(vpin->digital_read() == saved_bounce_logic_state){
                    longpress_timer.start();
                    if(multiclick_timer.get_time_passed() <= multiclick_interspace)
                        multiclick_count++;
                    else {
                        multiclick_count = 0;
                        for (auto &&event : this->multiclick_events){
                            event.done = false;
                        }
                    }
                    this->behaviors[active_behavior_id]->event_set[on_press]->operator()();
                    state = Button::pressed;
                } else
                    state = Button::idle;
            }
            break;
        case Button::pressed:
            if(vpin->digital_read() == !saved_bounce_logic_state){
                state = Button::post_bounce_testing;
                debounce_timer.start();
            } else {
                this->behaviors[active_behavior_id]->event_set[on_hold]->operator()();
                
                for (auto &&event : this->longpress_events){
                    if(!event.done && longpress_timer.have_passed(event.execute_at)){
                        event.event->operator()();
                        event.done = true;
                    }
                }
            }
                
            break;
        case Button::post_bounce_testing:
            if(debounce_timer.have_passed(30)){
                if(vpin->digital_read() == !saved_bounce_logic_state){
                    last_press_time = longpress_timer.get_time_passed();

                    for (auto &&event : this->longpress_events){
                        event.done = false;
                    }
                    
                    longpress_timer.clear();
                    multiclick_timer.start();
                    
                    this->behaviors[active_behavior_id]->event_set[on_release]->operator()();
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