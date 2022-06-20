#ifndef _ENCODER_HPP_
#define _ENCODER_HPP_

#include "Arduino.h"
#include "memory"

#include "ILoopable.hpp"
#include "Runtime.hpp"
#include "Params.hpp"

#include "Named.hpp"
#include "VPin.hpp"
#include "Button.hpp"


namespace async{
class Encoder
:   public ILoopable,
    public Named,
    public Behavable
{
private:
    enum Events {
        on_left = 0,
        on_right = 1,
    };

    std::unique_ptr<VPin> vpin_a;
    std::unique_ptr<VPin> vpin_b;

    const uint8_t read_encoder() const;

    uint8_t enc_val = 0;
    uint8_t enc_new_val = 0;
    
public:
    Encoder(const VPin& vpin_a,
            const VPin& vpin_b,
            const String& name = "",
            Params&& builder = [](){});

    Encoder(const uint8_t& pin_a,
            const uint8_t& pin_b,
            const String& name = "",
            Params&& builder = [](){});

    virtual ~Encoder();

    void set_on_left(std::function<void(void)>&& on_left_event);

    void set_on_right(std::function<void(void)>&& on_right_event);

    virtual void loop() override;
};
}
#endif
