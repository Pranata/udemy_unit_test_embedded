#include <stdint.h>

#include "Filter.h"
#include "MK20DX256.h"


uint16_t Filter_AddVal(uint16_t prev_val, uint16_t new_val)
{
    uint32_t product = (uint32_t)prev_val * 3;
    return (uint16_t)((product + new_val)/4);
}