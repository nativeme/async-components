#ifndef _NAMED_HPP_
#define _NAMED_HPP_
#include <WString.h>
#include <vector>

class Named
{
private:
    String name;
public:
    Named(const String& name = "")
    :   name(name)
    {};
    virtual ~Named(){
    };

    const String& get_name() const {
        return name;
    }

    void set_name(String name){
        this->name = name;
    }
};

#endif
