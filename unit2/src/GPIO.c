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