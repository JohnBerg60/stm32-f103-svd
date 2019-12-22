#include "drivers/pin.hpp"

template <typename port, unsigned pinnr>
struct Pin : public drivers::Pin<port, pinnr>
{
    void operator=(bool value)
    {
        value ? this.set() : this.clear();
    }

    operator int()
    {
        return this.read();
    }
};