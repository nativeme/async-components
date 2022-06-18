#include "Behavable.hpp"

/*
    Behavior implementation:
*/

Behavior::Behavior(const uint8_t& def_behavior_event_count, 
            const uint8_t& id, 
            const std::vector<IEvent*>& events_vec,
            const String& name)
:   id(id),
    name(name),
    events_vec(events_vec)
{
    if(events_vec.size() < def_behavior_event_count){
        this->events_vec.resize(def_behavior_event_count);
    }
}

Behavior::~Behavior(){
    for (auto &&event_ptr : events_vec){
        delete event_ptr;
    }
}

const uint8_t& Behavior::get_id() const{
    return this->id;
}

const String& Behavior::get_name() const {
    return this->name;
}

void Behavior::set_name(const String& name){
    this->name = name;
}


/*
    Behavable abstract class implementation:
*/

uint8_t Behavable::new_behavior_no(){
    return behavior_counter++;
}

Behavable::Behavable(const uint8_t& def_behavior_event_count)
:   def_behavior_event_count(def_behavior_event_count),
    behavior_counter(0),
    active_behavior_no(0)
{
    new_behavior("def");
}
Behavable::~Behavable(){
    for (auto &&behavior_ptr : behaviors_vec){
        delete behavior_ptr;
    }
}

Behavior& Behavable::get_active_behavior(){
    return *behaviors_vec[active_behavior_no];
}

void Behavable::new_behavior(const String& name){
    this->behaviors_vec.push_back(new Behavior(this->def_behavior_event_count, new_behavior_no(), {} , name));
    
    auto& bh = **std::next(behaviors_vec.end(), -1);
    for (size_t i = 0; i < this->def_behavior_event_count; i++){
        bh.events_vec[i] = new SingleEvent();
    }
}

void Behavable::next_behavior(){
    if(active_behavior_no + 2 > (uint8_t)behaviors_vec.size())
        active_behavior_no = 0;
    else
        ++active_behavior_no;
};

void Behavable::last_behavior(){
    if(active_behavior_no - 1 < 0)
        active_behavior_no = behaviors_vec.size() - 1;
    else 
        --active_behavior_no;
};

