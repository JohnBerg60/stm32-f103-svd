#include "drivers/register.hpp"
#include "drivers/gpio.hpp"

namespace HW
{

template <typename Port, unsigned pinnr>
struct Pin : public Register
{
    static constexpr unsigned mask{1 << pinnr};

    Pin()
    {
        Port::clockEnable();
        write<Port::CRH>(1 << 20);
    }

    void set()
    {
        write<Port::BSRR>(mask);
    }

    void clear()
    {
        write<Port::BSRR>(mask << 16);
    }

    bool read()
    {
        return Register::read<Port::IDR>() bitand mask;
    }
};

} // namespace HW