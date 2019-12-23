#include "drivers/register.hpp"
#include "drivers/gpio.hpp"

namespace drivers
{

template <typename port, unsigned pinnr>
struct Pin : public Register
{
    static constexpr unsigned mask{1 << pinnr};

    Pin()
    {
        port::clockEnable();
        regwrite<port::CRH>(1 << 20);
    }

    void set()
    {
        regwrite<port::BSRR>(mask);
    }

    void clear()
    {
        regwrite<port::BSRR>(mask << 16);
    }

    bool read()
    {
        return regread<port::IDR>() bitand mask;
    }

    void toggle()
    {
        togglebits<port::ODR>(mask);
    }
};

} // namespace drivers