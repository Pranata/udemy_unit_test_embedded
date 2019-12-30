#include "GPIO.h"
#include "MK20DX256.h"

int GPIO_SetPinAsOutput (uint8_t pin)
{
    if (pin >= 32)
        return GPIO_FAIL;

    PORTC.PDDR |= BIT_TO_MASK(pin);
    return GPIO_OK;
}

int GPIO_SetPinAsInput (uint8_t pin)
{
    if (pin >= 32)
        return GPIO_FAIL;

    PORTC.PDDR &= ~(BIT_TO_MASK(pin));
    return GPIO_OK;
}

int GPIO_SetPin (uint8_t pin)
{
    uint32_t mask = BIT_TO_MASK(pin);

    if (pin >= 32)
        return GPIO_FAIL;

    PORTC.PSOR = mask;

    if (!(PORTC.PDDR & mask))
    {
        PORTC.PDDR |= mask;
    }

    return GPIO_OK;
}

int GPIO_ClearPin (uint8_t pin)
{
    return GPIO_FAIL;
}