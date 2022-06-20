#include "Behavable.hpp"

/*
    Behavior (Behavable subclass) implementation:
*/

Behavable::Behavior::Behavior(Behavable* parent_behavable_obj,
                              const uint8_t& id, 
                              const std::vector<IEvent*>& events_vec,
                              const String& name)
:   parent_behavable_obj(parent_behavable_obj),
    id(id),
    name(name),
    events_vec(events_vec)
{
    if(events_vec.size() < parent_behavable_obj->def_behavior_event_count){
        this->events_vec.resize(parent_behavable_obj->def_behavior_event_count);
    }
}

Behavable::Behavior::~Behavior(){
    for (auto &&event_ptr : events_vec){
        delete event_ptr;
    }
}

const uint8_t& Behavable::Behavior::get_id() const {
    return this->id;
}

const String& Behavable::Behavior::get_name() const {
    return this->name;
}

void Behavable::Behavior::set_name(const String& name){
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

Behavable::Behavior& Behavable::get_active_behavior(){
    return *behaviors_vec[active_behavior_no];
}

Behavable::Behavior& Behavable::new_behavior(const String& name){
    this->behaviors_vec.push_back(new Behavior(this, new_behavior_no(), {} , name));
    
    auto& bh = **std::next(behaviors_vec.end(), -1);
    for (size_t i = 0; i < this->def_behavior_event_count; i++){
        bh.events_vec[i] = new SingleEvent();
    }

    return bh;
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

void Behavable::activate_behavior_by_name(const String& name){
    auto found = std::find_if(behaviors_vec.begin(), behaviors_vec.end(), [&name](const Behavior* behavior){
        return behavior->get_name() == name;
    });

    if(found != behaviors_vec.end()){
        this->active_behavior_no = (*found)->get_id();
    };
}

void Behavable::activate_behavior_by_id(const uint8_t& id){
    active_behavior_no = id;
}

/*
    Behavable group implementation.
    Templated constructor implementation in .hpp file.
*/

BehavableGroup::~BehavableGroup(){}

void BehavableGroup::next_behavior() const {
    for (auto &&member : *this){
        member->next_behavior();
    }
}

void BehavableGroup::last_behavior() const {
    for (auto &&member : *this){
        member->last_behavior();
    }
}

void BehavableGroup::activate_behavior_by_name(const String& name) const {
    for (auto &&member : *this){
        member->activate_behavior_by_name(name);   
    }
}

void BehavableGroup::activate_behavior_by_id(const uint8_t& id) const {
    for (auto &&member : *this){
        member->activate_behavior_by_id(id);
    }
}