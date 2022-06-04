/**
 * @file Button.hpp
 * @author Łukasz Kaniak - nativeme (lukaszkaniak@gmail.com)
 * @brief 
 * Async button implementation.
 * @version 0.1
 * @date 2022-05-29
 * @copyright Copyright (c) 2022
 */

#ifndef _BUTTON_HPP_
#define _BUTTON_HPP_

#include "Arduino.h"
#include "memory"

#include "VPin.hpp"
#include "Runtime.hpp"
#include "ILoopable.hpp"
#include "Timer.hpp"
#include "EventTimer.hpp"
#include "Libutils.hpp"

#include "Named.hpp"
#include "Behavable.hpp"

namespace async{
class Button
:   public Named,
    public Behavable,
    public ILoopable
{
protected:
    const std::unique_ptr<VPin> vpin;
    uint8_t state = 0;
    bool saved_bounce_logic_state = 0;
    uint32_t last_press_time = 0;
    uint8_t multiclick_interspace = 170;
    uint8_t multiclick_count = 0;

    enum State {
        idle = 0,
        pre_bounce_testing = 1,
        pressed = 2,
        post_bounce_testing = 3
    };

    enum Events {
        on_press = 0,
        on_release = 1,
        on_hold = 2,
        on_hold_after_set = 3
    };

    struct MultiClickEvent{
        uint8_t click_target;
        std::function<void(void)>* event;
        bool done;
        MultiClickEvent(uint8_t click_target, std::function<void(void)>* event)
        :   click_target(click_target),
            event(event),
            done(false)
        {}
    };

    struct LongPressEvent{
        uint32_t execute_at;
        std::function<void(void)>* event;
        bool done;
        LongPressEvent(uint32_t execute_at, std::function<void(void)>* event)
        :   execute_at(execute_at),
            event(event),
            done(false)
        {};
    };
    
    std::vector<MultiClickEvent> multiclick_events;
    std::vector<LongPressEvent>  longpress_events;

public:
    async::Timer debounce_timer   = async::Timer(UINT32_MAX / 2);
    async::Timer multiclick_timer = async::Timer(UINT32_MAX / 2);
    async::Timer longpress_timer = async::EventTimer(UINT32_MAX / 2);

    /**
     * @brief Create Button based on existing VPin.
     * @param vpin Existing VitualPin entity.
     * @param name String name for button. (optional - defaults to "")
     * @param standard_behavior Set of lambda functions that represent
     * on_press, on_hold and on_release. (optional - defaults to {})
     */
    Button(const VPin& vpin,
           const String& name = "",
           std::vector<std::function<void(void)>*>&& standard_behavior = {});
    
    // Button(const VPin& vpin,
    //        const Params&& builder);

    /**
     * @brief Create Button based on physical pin represented as integer number.
     * @param pin Pin number.
     * @param name String name for button. (optional - defaults to "")
     * @param standard_behavior Set of lambda functions that represent
     * on_press, on_hold and on_release. (optional - defaults to {})
     */
    Button(const uint8_t pin, 
           const String& name = "", 
           std::vector<std::function<void(void)>>&& standard_behavior = {});
    
    // Button(const uint8_t pin,
    //        const Params&& builder);

    virtual ~Button();
    /**
     * @brief Sets event that will be triggered at the
     * press moment. Affects current behavior.
     * @param on_press_event lambda function.
     */
    void set_on_press(std::function<void(void)>& on_press_event);
    /**
     * @brief Sets event that will be triggered at the
     * release moment. Affects current behavior.
     * @param on_release_event lambda function.
     */
    void set_on_release(std::function<void(void)>&& on_release_event);
    /**
     * @brief Sets codeblock that will be executed in async loop when button
     * will be held. Affects current behavior.
     * @param on_hold_codeblock lambda function.
     */
    void set_on_hold_loop(std::function<void(void)>& on_hold_codeblock);
    /**
     * @brief At the moment, it works regardless of the chosen behavior.
     * Sets event after certain timespan of holding button.
     * set_on_hold_after(time, [](){ some logic here })
     * @param press_duration timespan after with event will be triggered.
     * @param on_hold_event event function lambda.
     */
    void set_on_hold_after(const uint32_t& press_duration, std::function<void(void)>&& on_hold_event);
    /**
     * @brief Sets new multiclick event.
     * @param click_target Event will be triggered after this amount of clicks.
     * @param event event function lambda.
     */
    void set_on_multiclicks(const uint8_t& click_target, std::function<void(void)>&& multiclick_event);
    /**
     * @brief Sets maximum interspace between
     * multiclick is registered. (This value defaults to 170 ms.)
     * @param multiclick_interspace 
     */
    void set_multiclick_interspace(const uint8_t& multiclick_interspace);
  
    /**
     * @brief Gets current press duration time.
     * Useful with "set_on_hold()" method. 
     * @return uint32_t 
     */
    uint32_t get_current_press_duration() const;
    /**
     * @brief Gets last press duration time.
     * Useful with "on_release()" method. 
     * @return const uint32_t& 
     */
    const uint32_t& get_last_press_time();
    virtual void loop() override;
};
}
#endif
