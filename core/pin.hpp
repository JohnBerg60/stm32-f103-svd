#include "drivers/pin.hpp"

template <typename port, unsigned pinnr>
struct Pin : public HW::Pin<port, pinnr>
{
    void toggle()
    {
        this->read() ? this->clear() : this->set();
    }

    void operator=(bool value)
    {
        value ? this.set() : this.clear();
    }

    operator int()
    {
        return this.read();
    }
};