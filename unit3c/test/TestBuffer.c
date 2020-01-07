#include <stdint.h>
#include "unity.h"
#include "Buffer.h"
#include "MK20DX256.h"

void test_BufferPut_and_Get_should_WorkTogetherToInsertAndExtractAValue(void)
{
    const uint16_t data[] = {0, 1, 0x5A, 0x7FFF, 0xFFFF};
    uint16_t fetched;
    int i;

    for (i = 0; i < ( sizeof(data)/sizeof(uint16_t) ); i++)
    {
        TEST_ASSERT_EQUAL(0, BufferPut(data[i]));
        TEST_ASSERT_EQUAL(0, BufferGet(&fetched));
        TEST_ASSERT_EQUAL_HEX16(data[i], fetched);
    }
}

void test_BufferGet_should_ReturnErrorIfCalledWhenEmpty(void)
{
    uint16_t fetched;

    TEST_ASSERT_NOT_EQUAL(0, BufferGet(&fetched));
}

void test_BufferPut_should_ReturnErrorIfCalledWhenFull(void)
{
    int i;
    int expected[BUF_SIZE - 1];
    int actual[BUF_SIZE - 1];

    for (i = 0; i < BUF_SIZE - 1; i++)
    {
        expected[i] = 0;
        actual[i] = BufferPut(i);
    }

    TEST_ASSERT_EQUAL_INT_ARRAY(expected, actual, BUF_SIZE - 1);
    TEST_ASSERT_NOT_EQUAL(0, BufferPut(1));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_BufferPut_and_Get_should_WorkTogetherToInsertAndExtractAValue);
    RUN_TEST(test_BufferGet_should_ReturnErrorIfCalledWhenEmpty);
    RUN_TEST(test_BufferPut_should_ReturnErrorIfCalledWhenFull);
    return UNITY_END();
}
