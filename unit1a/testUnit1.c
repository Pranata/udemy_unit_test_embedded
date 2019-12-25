
#include "unity.h"

void test_TheFirst(void)
{
    TEST_ASSERT_EQUAL(1, 1);
    TEST_ASSERT_MESSAGE(1 == 2, "1 is not the same as 2");
}

void test_TheSecond(void)
{
    TEST_ASSERT_FALSE(0);
}

void test_TheThird(void)
{
    TEST_ASSERT_TRUE(6);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_TheFirst);
    RUN_TEST(test_TheSecond);
    RUN_TEST(test_TheThird);
    return UNITY_END();
}
