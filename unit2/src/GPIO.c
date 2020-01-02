#include "GPIO.h"
#include "MK20DX256.h"


static uint32_t m_reverse_mask;


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

    if (m_reverse_mask & mask)
        PORTC.PCOR = mask;
    else
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

uint32_t GPIO_ReadPort(void)
{
    return PORTC.PDIR ^ m_reverse_mask;
}

void GPIO_Init (void)
{
    PORTC.PDDR = 0x1012A000;
    PORTC.PSOR = 0x10102000;
    PORTC.PCOR = 0x00028000;

    m_reverse_mask = 0x00000022;
}