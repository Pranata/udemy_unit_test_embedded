#include <stdint.h>
#include <stdbool.h>
#include <string.h>
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

    BufferClear();
    
    for (i = 0; i < BUF_SIZE - 1; i++)
    {
        expected[i] = 0;
        actual[i] = BufferPut(i);
    }

    TEST_ASSERT_EQUAL_INT_ARRAY(expected, actual, BUF_SIZE - 1);
    TEST_ASSERT_NOT_EQUAL(0, BufferPut(1));
}

void test_BufferClear_should_ResetTheBuffer(void)
{
    int i;
    int expected[BUF_SIZE - 1];
    int actual[BUF_SIZE - 1];

    BufferClear();
    
    for (i = 0; i < BUF_SIZE - 1; i++)
    {
        expected[i] = 0;
        actual[i] = BufferPut(i);
    }
    TEST_ASSERT_EQUAL_INT_ARRAY(expected, actual, BUF_SIZE - 1);
    TEST_ASSERT_FALSE(BufferIsEmpty());
    TEST_ASSERT_TRUE(BufferIsFull());

    BufferClear();
    TEST_ASSERT_FALSE(BufferIsFull());
    TEST_ASSERT_TRUE(BufferIsEmpty());
}

void test_BufferIsFull_should_ReturnStatusCorrectly(void)
{
    int i;
    int expected[BUF_SIZE - 1];
    int actual[BUF_SIZE - 1];

    BufferClear();
    
    for (i = 0; i < BUF_SIZE - 1; i++)
    {
        expected[i] = 0;
        actual[i] = BufferPut(i);
    }

    TEST_ASSERT_EQUAL_INT_ARRAY(expected, actual, BUF_SIZE - 1);
    TEST_ASSERT_FALSE(BufferIsEmpty());
    TEST_ASSERT_TRUE(BufferIsFull());
}

void test_BufferIsEmpty_should_ReturnStatusCorrectly(void)
{
    int i;
    int expected[BUF_SIZE - 1];
    int actual[BUF_SIZE - 1];

    BufferClear();
    
    TEST_ASSERT_FALSE(BufferIsFull());
    TEST_ASSERT_TRUE(BufferIsEmpty());
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_BufferPut_and_Get_should_WorkTogetherToInsertAndExtractAValue);
    RUN_TEST(test_BufferGet_should_ReturnErrorIfCalledWhenEmpty);
    RUN_TEST(test_BufferPut_should_ReturnErrorIfCalledWhenFull);
    RUN_TEST(test_BufferClear_should_ResetTheBuffer);
    RUN_TEST(test_BufferIsFull_should_ReturnStatusCorrectly);
    RUN_TEST(test_BufferIsEmpty_should_ReturnStatusCorrectly);
    return UNITY_END();
}
