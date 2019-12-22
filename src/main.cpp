#include <cstdint>

// stm32g031K8

//Reg.hpp
//#pragma once

#include <cstdint>
#include <cstdbool>

// struct/class/namespace -     Upper
// class methods -              lower or lowerUpper
// class constants -            UPPER_
// template parameters -        Upper_
// namespace w/only enums -     UPPER
// enums -                      UPPER
//

//=================================================================================
// generic volatile regiter bit/read/write functions, basic version
// compile time assert for alignment, only compile time known addresses will work
//=================================================================================
struct Reg
{

    //write
    //write access size is matched to Val_ type size
    template <unsigned Addr_, typename Val_>
    static constexpr void regWr(const Val_ val)
    {
        constexpr unsigned ALIGNMASK_{sizeof(Val_) > 4 ? 3 : sizeof(Val_) - 1};
        static_assert(not(Addr_ bitand ALIGNMASK_), "Alignment error in Reg::regWr");
        *(reinterpret_cast<volatile Val_ *>(Addr_)) = val;
    }

    //read
    //32bit default, need to specify if 64, 16 or 8 bit -
    // read 8 bit - reg<0x1005, uint8_t>()
    // read 16 bit - reg<0x1002, uint16_t>()
    // read 64 bit - reg<0x1016, uint64_t>()
    template <unsigned Addr_, typename Ret_ = uint32_t>
    static constexpr Ret_ regRd()
    {
        constexpr unsigned ALIGNMASK_{sizeof(Ret_) > 4 ? 3 : sizeof(Ret_) - 1};
        static_assert(not(Addr_ bitand ALIGNMASK_), "Alignment error in Reg::regRd");
        return *(reinterpret_cast<volatile Ret_ *>(Addr_));
    }

    //bits, will use underlying type of mask value to determine what kind
    //of register access (8,16,32,64 bit)
    //only do if mask not 0

    //clear bit(s) in maskC, set all bit(s) in maskS (to set multi-bit values)
    template <unsigned Addr_, typename Mtyp_>
    static constexpr void setmask(const Mtyp_ maskC, const Mtyp_ maskS)
    {
        //use tmp so only get one write (vs putting in regWr argument)
        Mtyp_ tmp = regRd<Addr_, Mtyp_>() bitand compl maskC;
        regWr<Addr_, Mtyp_>(tmp bitor maskS);
    }

    //clear all bits in mask
    template <unsigned Addr_, typename Mtyp_>
    static constexpr void clrbit(const Mtyp_ mask)
    {
        if (mask)
            regWr<Addr_, Mtyp_>(regRd<Addr_, uint32_t>() bitand compl mask);
    }

    //set all bit(s) in mask
    template <unsigned Addr_, typename Mtyp_>
    static constexpr void setbit(const Mtyp_ mask)
    {
        if (not mask)
            return;
        regWr<Addr_, Mtyp_>(regRd<Addr_, Mtyp_>() bitor mask);
    }
    //set all bit(s) in mask to tf (1,0)
    //allows setbit to also do clrbit, so do not need the if/else in calling code
    template <unsigned Addr_, typename Mtyp_>
    static constexpr void setbit(const Mtyp_ mask, bool tf)
    {
        if (not mask)
            return;
        if (tf)
            setbit<Addr_>(mask);
        else
            clrbit<Addr_>(mask);
    }

    //toggle all bit(s) in mask
    template <unsigned Addr_, typename Mtyp_>
    static constexpr void togbit(const Mtyp_ mask)
    {
        if (not mask)
            return;
        regWr<Addr_, Mtyp_>(regRd<Addr_, Mtyp_>() xor mask);
    }

    //if any bit is set in mask position(s), return true
    template <unsigned Addr_, typename Mtyp_>
    static constexpr bool anybit(const Mtyp_ mask)
    {
        return regRd<Addr_, Mtyp_>() bitand mask;
    }
};
//=================================================================================

//Pins.hpp
//#pragma once

#include <cstdint>
#include <cstdbool>
//#include "Reg.hpp"

//stm32g031

//=================================================================================
// Pins namespace, used for Gpio template paramters
// enums for Pin port and pin number -
//      PIN - low 4 bits is pin number, upper bits are port letter
//=================================================================================
//set pins as needed for mcu in use (incomplete, really only need PC6 for now)
namespace PINS
{

enum PIN : unsigned
{
    PA0,
    PA1,
    PA13 = 13,
    PA14,
    PB0 = 16,
    PB1,
    PC0 = 32,
    PC1,
    PC6 = 38, //PC6 = led on nucleo-32 board
    PC13 = 45,
    PD0 = 48,
    PD1,
    PF0 = 80,
    PF1
};

enum MODE : unsigned
{
    INPUT,
    OUTPUT,
    ALTERNATE,
    ANALOG
};

enum OTYPE : unsigned
{
    PUSHPULL,
    ODRAIN
};

enum OSPEED : unsigned
{
    SPEEDLOW,
    SPEEDMED,
    SPEEDHI,
    SPEEDVERYHI
};

enum ONVAL : unsigned
{
    LOWISON,
    HIGHISON
};

}; // namespace PINS
//=================================================================================

//Rcc.hpp
//=================================================================================
// Rcc - reset & clock control
// mimimal for Gpio use
//=================================================================================
struct Rcc : private Reg
{

    enum : unsigned
    {
        RCC_BASE = 0x40021000UL,
        BASE = RCC_BASE,
        CR = BASE,
        CFGR = BASE + 0x4,
        CIR = BASE + 0x8,
        APB2RSTR = BASE + 0xC,
        APB1RSTR = BASE + 0x10,
        AHBENR = BASE + 0x14,
        APB2ENR = BASE + 0x18,
        APB1ENR = BASE + 0x1C,
        BDCR = BASE + 0x20,
        CSR = BASE + 0x24
    };

    static constexpr void gpioClockEn(PINS::PIN pin, bool tf = true)
    {
        setbit<APB2ENR>(1 << (pin / 16) << 2, tf);
    }

    static constexpr void gpioSleepEn(PINS::PIN pin, bool tf = true)
    {
        setbit<APB2ENR>(1 << (pin / 16), tf);
    }
};
//=================================================================================

//=================================================================================
// Gpio
//=================================================================================
template <PINS::PIN Pin_, PINS::MODE Mode_ = PINS::ANALOG, PINS::ONVAL OnVal_ = PINS::HIGHISON>
class Gpio : private Reg
{

    //make functions more readable, no cost
    static constexpr unsigned PIN_{Pin_ bitand 15};         //pin 0-15
    static constexpr unsigned PINMASK_{1 << PIN_};          //bitmask of pin
    static constexpr unsigned PORT_{Pin_ / 16};             //port A-D, F
    static constexpr unsigned PORTMASK_{1 << PORT_};        //bitmask of port
    static constexpr unsigned IOPORT_BASE_{0x40010800UL};   //base address of gpio
    static constexpr unsigned IOPORT_SPACING_{0x400};       //port address spacing
    static constexpr unsigned PIN2BIT_{PIN_ * 2};           //registers with 2bit values
    static constexpr unsigned PINMASK2BIT_{3 << PIN2BIT_};  //mask of the 2 bit value
    static constexpr unsigned AFROFFSET_{(PIN_ / 8)};       //register offset 0 or 1
    static constexpr unsigned AFRPIN4BIT_{(PIN_ % 8) << 4}; //pin bit position, 0,4,8,...

    //gpio registers
    enum : unsigned
    {
        BASE = IOPORT_BASE_ + (IOPORT_SPACING_ * PORT_),
        CRL = BASE,
        CRH = BASE + 0x4,
        IDR = BASE + 0x8,
        ODR = BASE + 0xC,
        BSRR = BASE + 0x10,
        BRR = BASE + 0x14,
        LCKR = BASE + 0x18,
    };

public:
    Gpio()
    {
        Rcc::gpioClockEn(Pin_);
        asm("nop"); //2 clocks to sync
        asm("nop"); //so no reg writes just yet
        //deinit(); //just call manually if ever needed
        off();
        mode(Mode_);
    }

    static void
    deinit()
    {
        //PA13,PA14 have non-0 register defaults
        //pin low
        low();
        //reset alt func, but do not set pin to use
        altfunc<0>(false);
        //set mode
        mode(Pin_ == PINS::PA13 or Pin_ == PINS::PA14 ? PINS::ALTERNATE : PINS::ANALOG);
        //set pull
        if (Pin_ == PINS::PA13)
            pullUp();
        else if (Pin_ == PINS::PA14)
            pullDown();
        else
            pullOff();
        //set speed
        if (Pin_ == PINS::PA13)
            outSpeed(PINS::SPEEDVERYHI);
        else
            outSpeed(PINS::SPEEDLOW);
        //push pull output mode (not open drain)
        pushPull();
    }

    //set
    static void high()
    {
        regWr<BSRR>(PINMASK_);
    }

    static void low()
    {
        regWr<BRR>(PINMASK_);
    }

    static void on()
    {
        if (OnVal_ == PINS::HIGHISON)
            high();
        else
            low();
    }

    static void on(bool tf)
    {
        if (tf)
            on();
        else
            off();
    }

    static void off()
    {
        if (OnVal_ == PINS::LOWISON)
            high();
        else
            low();
    }

    static void toggle()
    {
        togbit<ODR>(PINMASK_);
    }

    //get
    static bool isOn()
    {
        return pinVal() == OnVal_;
    }

    static bool isOff()
    {
        return not isOn();
    }

    static bool isHigh()
    {
        return pinVal() == 1;
    }

    static bool isLow()
    {
        return not isHigh();
    }

    static bool pinVal()
    {
        return anybit<IDR>(PINMASK_);
    }

    static bool latVal()
    {
        return anybit<ODR>(PINMASK_);
    }

    //set properties
    static void mode(PINS::MODE e)
    {
        //setmask<MODER>(PINMASK2BIT_, e << PIN2BIT_);
        setbit<CRH>(0x33333333);
    }

    static void
    pushPull()
    {
        //clrbit<OTYPER>(PINMASK_);
    }

    static void
    openDrain()
    {
        //setbit<OTYPER>(PINMASK_);
    }

    static void
    outSpeed(PINS::OSPEED e)
    {
        //setmask<OSPEEDR>(PINMASK2BIT_, e << PIN2BIT_);
    }

    static void
    pullUp()
    {
        //setmask<PUPDR>(PINMASK2BIT_, 1 << PIN2BIT_);
    }

    static void
    pullDown()
    {
        //setmask<PUPDR>(PINMASK2BIT_, 2 << PIN2BIT_);
    }

    static void
    pullOff()
    {
        //clrbit<PUPDR>(PINMASK2BIT_);
    }

    //enable sleep mode for the port
    static void
    sleepEnable()
    {
        Rcc::gpioSleepEn(Pin_);
    }

    //alt functions - simple version, just need a raw value 0-15
    template <uint8_t v> //template, forces compile time assert 0-15
    static void
    altfunc(bool setmode = true)
    {
        static_assert(v <= 15, "invalid function parameter in Gpio::altfunc");
        //clrbit<AFRL + AFROFFSET_>(15 << AFRPIN4BIT_);
        //setbit<AFRL + AFROFFSET_>(v << AFRPIN4BIT_);
        //function is set, now can switch to it if wanted (default)
        if (setmode)
            mode(PINS::ALTERNATE);
    }
};
//=================================================================================

//delay of some unkown time
static void pause(uint32_t n = 65536)
{
    for (volatile uint32_t i = 0; i < n; i++)
    {
        __asm("nop");
    }
}

//a function that takes a gpio pin, blinks n times
template <typename T>
static void blink(T pin, uint8_t n, uint32_t t = 65536)
{
    for (int nn = n * 2; nn--;)
    {
        pin.toggle();
        pause(t);
    }
}

int main()
{

    using namespace PINS;
    Gpio<PC13, OUTPUT, LOWISON> led;

    for (;;)
    {
        led.on();
        pause(65536);
        led.off();
        pause(65536);
    }

    for (;;)
    {
        // blink( led, 20 );
        // pause(65536*100);
    }
}
