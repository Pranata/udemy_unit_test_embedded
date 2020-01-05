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


void test_Filter_AddVal_should_HandleCloseToZeroOrMaxValueCalculations(void)
{
    //previous value close to 0
    TEST_ASSERT_EQUAL_UINT16(25, Filter_AddVal(0, 100));
    TEST_ASSERT_EQUAL_UINT16(250, Filter_AddVal(1, 1000));
    TEST_ASSERT_EQUAL_UINT16(10, Filter_AddVal(10, 10));

    //previous value near the max 16-bit unsigned
    TEST_ASSERT_EQUAL_UINT16(49175, Filter_AddVal(65534, 100));
    TEST_ASSERT_EQUAL_UINT16(49401, Filter_AddVal(65535, 1000));
    TEST_ASSERT_EQUAL_UINT16(49153, Filter_AddVal(65534, 10));

    //new value close to 0
    TEST_ASSERT_EQUAL_UINT16(75, Filter_AddVal(100, 0));
    TEST_ASSERT_EQUAL_UINT16(750, Filter_AddVal(1000, 1));
    TEST_ASSERT_EQUAL_UINT16(10, Filter_AddVal(10, 10));

    //new value near the max 16-bit unsigned
    TEST_ASSERT_EQUAL_UINT16(16458, Filter_AddVal(100, 65534));
    TEST_ASSERT_EQUAL_UINT16(17133, Filter_AddVal(1000, 65535));
    TEST_ASSERT_EQUAL_UINT16(16391, Filter_AddVal(10, 65534));
}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_Filter_AddVal_should_OutputPrevVal_if_PrevValAndNewValMatch);
    RUN_TEST(test_Filter_AddVal_should_HandleSimpleMiddleRangeCalculations);
    RUN_TEST(test_Filter_AddVal_should_HandleCloseToZeroOrMaxValueCalculations);
    return UNITY_END();
}
