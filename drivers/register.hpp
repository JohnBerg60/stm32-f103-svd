#pragma once

struct Register
{
    template <unsigned Address, typename returnValue = unsigned>
    static constexpr returnValue regread()
    {
        return *(reinterpret_cast<volatile returnValue *>(Address));
    }

    template <unsigned Address, typename Value>
    static constexpr void regwrite(const Value value)
    {
        *(reinterpret_cast<volatile Value *>(Address)) = value;
    }

    //set all bit(s) in mask
    template <unsigned Address, typename MaskType>
    static constexpr void setbits(const MaskType mask)
    {
        if (not mask)
            return;
        regwrite<Address, MaskType>(regread<Address>() bitor mask);
    }

    //clear all bit(s) in mask
    template <unsigned Address, typename MaskType>
    static constexpr void clearbits(const MaskType mask)
    {
        if (not mask)
            return;
        regwrite<Address, MaskType>(regread<Address>() bitand mask);
    }

    //toggle all bit(s) in mask
    template <unsigned Address, typename MaskType>
    static constexpr void togglebits(const MaskType mask)
    {
        if (not mask)
            return;
        regwrite<Address, MaskType>(regread<Address, MaskType>() xor mask);
    }
};