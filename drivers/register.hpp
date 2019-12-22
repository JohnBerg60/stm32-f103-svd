#pragma once

struct Register
{
    template <unsigned Address, typename returnValue = unsigned>
    static constexpr returnValue read()
    {
        return *(reinterpret_cast<volatile returnValue *>(Address));
    }

    template <unsigned Address, typename Value>
    static constexpr void write(const Value value)
    {
        *(reinterpret_cast<volatile Value *>(Address)) = value;
    }

    //set all bit(s) in mask
    template <unsigned Address, typename MaskType>
    static constexpr void setbit(const MaskType mask)
    {
        if (not mask)
            return;
        write<Address, MaskType>(read<Address>() bitor mask);
    }

    //clear all bit(s) in mask
    template <unsigned Address, typename MaskType>
    static constexpr void clearbit(const MaskType mask)
    {
        if (not mask)
            return;
        write<Address, MaskType>(read<Address>() bitand mask);
    }
};