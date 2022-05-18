#ifndef _NAMED_HPP_
#define _NAMED_HPP_
#include "Arduino.h"

namespace async{
class Named
{
private:
    String name;
public:
    Named();
    virtual ~Named();

    String& get_name() {
        return name;
    }

    void set_name(String name){
        this->name = name;
    }
};
}
#endif
