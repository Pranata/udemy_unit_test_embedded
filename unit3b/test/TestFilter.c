#include <stdint.h>

#include "unity.h"
#include "Filter.h"
#include "MK20DX256.h"

void test_Filter_AddVal_should_OutputPrevVal_if_PrevValAndNewValMatch(void)
{
    TEST_ASSERT_EQUAL_HEX16(0, Filter_AddVal(0, 0));
    TEST_ASSERT_EQUAL_HEX16(0xFFFF, Filter_AddVal(0xFFFF, 0xFFFF));
    TEST_ASSERT_EQUAL_HEX16(0x8000, Filter_AddVal(0x8000, 0x8000));
    TEST_ASSERT_EQUAL_HEX16(0x7FFF, Filter_AddVal(0x7FFF, 0x7FFF));
}


void test_Filter_AddVal_should_HandleSimpleMiddleRangeCalculations(void)
{
    TEST_ASSERT_EQUAL_UINT16(775, Filter_AddVal(1000, 100));
    TEST_ASSERT_EQUAL_UINT16(325, Filter_AddVal(100, 1000));
}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_Filter_AddVal_should_OutputPrevVal_if_PrevValAndNewValMatch);
    RUN_TEST(test_Filter_AddVal_should_HandleSimpleMiddleRangeCalculations);
    return UNITY_END();
}
