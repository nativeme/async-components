#include "Button.hpp"
namespace async{

    Button::Button(const VPin& vpin,
                   const String& name,
                   Params&& builder)
    :   Named(name),
        Behavable(3),
        vpin(std::make_unique<VPin>(vpin))
    {   
        builder.builder();
        this->vpin->pin_mode(INPUT_PULLUP);
        Runtime::loopables.push_back(this);
    };

    Button::Button(const uint8_t pin, 
                   const String& name,
                   const Params&& builder)
    :   Named(name),
        Behavable(3),   
        vpin(new VPin(pin))
    {
        builder.builder();
        this->vpin->pin_mode(INPUT_PULLUP);
        Runtime::loopables.push_back(this);
    };

    Button::~Button(){
    };

    void Button::set_on_press(std::function<void(void)>&& on_press_event){
        auto ev = new SingleEvent(std::move(on_press_event));
        this->get_active_behavior().events_vec[on_press] = ev;
    }

    void Button::set_on_release(std::function<void(void)>&& on_release_event){
        auto ev = new SingleEvent(std::move(on_release_event));
        this->get_active_behavior().events_vec[on_release] = ev;
    }

    void Button::set_on_hold_loop(std::function<void(void)>&& on_hold_event){
        auto ev = new SingleEvent(std::move(on_hold_event));
        this->get_active_behavior().events_vec[on_hold] = ev;
    }

    void Button::set_on_hold_after(const uint32_t& press_duration, std::function<void(void)>&& longpress_event){
        auto ev = new LongPressEvent(this,
                                     this->get_active_behavior().get_id(),
                                     press_duration, 
                                     std::move(longpress_event));
        longpress_events.push_back(ev);
    };

    void Button::set_on_multiclicks(const uint8_t& click_target, std::function<void(void)>&& multiclick_event){
        auto ev = new MultiClickEvent(this,
                                      this->get_active_behavior().get_id(),
                                      click_target, 
                                      std::move(multiclick_event));
        multiclick_events.push_back(ev);
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
                if(!event->done && 
                    event->click_target == this->multiclick_count + 1 && 
                    multiclick_timer.get_time_passed() > multiclick_interspace){
                        event->execute();
                        event->done = true;
                    }
            }

            saved_bounce_logic_state = vpin->digital_read();
            if(saved_bounce_logic_state){
                debounce_timer.start();
                state = Button::pre_bounce_testing;
            }

            break;
        case Button::pre_bounce_testing:
            if(debounce_timer.have_passed(30)){
                if(vpin->digital_read() == saved_bounce_logic_state){
                    longpress_timer.start();

                    if(multiclick_timer.get_time_passed() <= multiclick_interspace)
                        multiclick_count++;
                    else {
                        multiclick_count = 0;
                        for (auto &&event : this->multiclick_events){
                            event->done = false;
                        }
                    }
                    
                    this->get_active_behavior().events_vec[on_press]->execute();

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
                this->get_active_behavior().events_vec[on_hold]->execute();

                for (auto &&event : this->longpress_events){
                    if(!event->done && longpress_timer.have_passed(event->execute_at)){
                        event->execute();
                        event->done = true;
                    }
                }
            }
                
            break;
        case Button::post_bounce_testing:
            if(debounce_timer.have_passed(30)){
                if(vpin->digital_read() == !saved_bounce_logic_state){
                    last_press_time = longpress_timer.get_time_passed();

                    for (auto &&event : this->longpress_events){
                        event->done = false;
                    }
                    
                    longpress_timer.clear();
                    multiclick_timer.start();
                    
                    this->get_active_behavior().events_vec[on_release]->execute();

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