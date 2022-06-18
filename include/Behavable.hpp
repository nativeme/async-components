/**
 * @file Behavable.hpp
 * @author Łukasz Kaniak - nativeme (lukaszkaniak@gmail.com)
 * @brief 
 * Abstract class that defines element that have it's own set of behaviors.
 * 
 * @version 0.1
 * @date 2022-06-18
 * @copyright Copyright (c) 2022
 */

#ifndef _BEHAVABLE_HPP_
#define _BEHAVABLE_HPP_

#include "Arduino.h"
#include "EventTypes.hpp"

class Behavior{
private:
    const uint8_t id;
    String name;
public:
    std::vector<IEvent*> events_vec;

    /**
     * @brief
     * Set of events that can be triggered in deriven object.
     * @param def_behavior_event_count 
     * Number of events (functions) that will be created with the behavior.
     * @param id 
     * Id of the behavior.
     * @param events_vec 
     * Set of events passed to the behavior.
     * @param name 
     * Name for the behavior.
     */
    Behavior(const uint8_t& def_behavior_event_count, 
             const uint8_t& id, 
             const std::vector<IEvent*>& events_vec,
             const String& name);
    
    ~Behavior();

    /**
     * @brief
     * Gets const reference to id of the behavior.
     * @return
     * const uint8_t& - Id od the behavior.
     */
    const uint8_t& get_id() const;

    /**
     * @brief
     * Gets const reference to the name of the behavior.
     * @return 
     * const String& - name of the behavior.
     */
    const String& get_name() const;
    
    /**
     * @brief 
     * Sets name of the behavior.
     * @param name 
     * New name for the behavior.
     */
    void set_name(const String& name);

};

class Behavable
{
private:
    uint8_t def_behavior_event_count;
    uint8_t behavior_counter;
    uint8_t active_behavior_no;

    /**
     * @brief 
     * Internal method that increments behavior's "id".
     * @return uint8_t - new behavior's id.
     */
    uint8_t new_behavior_no();

protected:
    std::vector<Behavior*> behaviors_vec;
public:
    /**
     * @brief 
     * Construct a new Behavable object
     * @param def_behavior_event_count
     * Default number of events (functions) that will be created with every new behavior.
     */
    Behavable(const uint8_t& def_behavior_event_count);

    virtual ~Behavable();

    /**
     * @brief 
     * Gets the active behavior object reference.
     * @return 
     * Behavior& - Reference to active behavior object.
     */
    Behavior& get_active_behavior();

    /**
     * @brief 
     * Creates new behavior for behavable entity.
     * @param name 
     * (optional) name for new behavior.
     */
    void new_behavior(const String& name = "");

    /**
     * @brief 
     * Changes behavior of behavable entity to next possible behavior.
     * If last behavior is selected changes behavior to first one.
     */
    void next_behavior();

    /**
     * @brief
     * Changes behavior of behavable entity to last possbile behavior.
     * If first behavior is selected changes behavior to last one.
     */
    void last_behavior();

    void activate_by_name(const String& name){
        auto found = std::find_if(behaviors_vec.begin(), behaviors_vec.end(), [&name](const Behavior* behavior){
            return behavior->get_name() == name;
        });

        if(found != behaviors_vec.end()){
            this->active_behavior_no = (*found)->get_id();
        };
    }
};

#endif
