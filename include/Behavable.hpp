#ifndef _BEHAVABLE_HPP_
#define _BEHAVABLE_HPP_

#include "Arduino.h"

class Behavable
{
private:
// protected:
public:
    class Behavior{
    public:
        std::vector<std::function<void()>*> event_set;
        String behavior_name;
        uint8_t id;
        
        Behavior(const std::vector<std::function<void()>*>& event_set = {},
                 const String& behavior_name = "",
                 uint8_t id = 0)
        :   event_set(event_set),
            behavior_name(behavior_name),
            id(id)
        {};
        virtual ~Behavior(){
            Serial.println("Behavior destroyed");
            for (auto &&event : event_set){
                delete event;
            }
        };
    };

    uint8_t active_behavior_id;
    std::vector<Behavior*> behaviors;
public:
    Behavable()
    :   active_behavior_id(0)
    {};
    virtual ~Behavable(){
        for (auto &&behavior : behaviors){
            delete behavior;
        }     
    };

    void activate_behavior(uint8_t behavior_id){
        active_behavior_id = behavior_id;
    }

    // void activate_behavior(const String& behavior_name){
    //     auto found = std::find_if(behaviors.begin(), behaviors.end(), [&behavior_name](const Behavior& beh){
    //         return beh.behavior_name == behavior_name;
    //     });
    //     if (found != behaviors.end());
    //         // active_behavior_id = (*found).
    // }

    uint8_t new_behavior(std::vector<std::function<void(void)>*>& event_set, 
                         const String& name = ""){
        behaviors.push_back(new Behavior(event_set, name, behaviors.size() + 1));
        return behaviors.size() - 1;
    }

    uint8_t get_active_behavior_id(){
        return active_behavior_id;
    };

    String& get_behavior_name(){
        return this->behaviors[active_behavior_id]->behavior_name;
    };

    uint8_t next_behavior(){
        if(active_behavior_id != behaviors.size() - 1)
            active_behavior_id++;
        else 
            active_behavior_id = 0;
        
        return active_behavior_id;
    };

    uint8_t last_behavior(){
        if(active_behavior_id != 0) 
            active_behavior_id--;
        else 
            active_behavior_id = behaviors.size() - 1;
        
        return active_behavior_id;
    };
};
#endif
