#include <limits.h>

#include "unity.h"
#include "Fib.h"

#define DIMENSION_OF(a)    ( sizeof(a) / sizeof(a[0]) )

void test_element0_should_return1(void)
{
    TEST_ASSERT_EQUAL_INT(1, Fibonacci_GetElement(0));
}

void test_element1_should_return1(void)
{
    TEST_ASSERT_EQUAL(1, Fibonacci_GetElement(1));
}

void test_element2_should_return2(void)
{
    TEST_ASSERT_EQUAL(2, Fibonacci_GetElement(2));
}

void test_part_of_the_sequence_style1(void)
{
    TEST_ASSERT_EQUAL(5, Fibonacci_GetElement(4));
    TEST_ASSERT_EQUAL(21, Fibonacci_GetElement(7));
    TEST_ASSERT_EQUAL(89, Fibonacci_GetElement(10));
}

void test_part_of_the_sequence_style2(void)
{
    int expected[] = {1, 1, 2, 3, 5, 8, 13, 21, 34};
    int i;

    for (i = 0; i < DIMENSION_OF(expected); i++)
    {
        TEST_ASSERT_EQUAL_INT(expected[i], Fibonacci_GetElement(i));
    }
}

void test_negative_value_should_return0(void)
{
    TEST_ASSERT_EQUAL_INT(0, Fibonacci_GetElement(-2));
    TEST_ASSERT_EQUAL_INT(0, Fibonacci_GetElement(-555));
    TEST_ASSERT_EQUAL_INT(0, Fibonacci_GetElement(INT_MIN));
}

void test_overrun_value_should_return0(void)
{
    const int first_overrun_element = 46;
    const int last_good_element = first_overrun_element - 1;

    int last_good_value = Fibonacci_GetElement(last_good_element);

    TEST_ASSERT_EQUAL_INT_MESSAGE(4, sizeof(int), "Mismatch architecture, change first_overrun_element and FIB_INDEX_MAX on Fib.c");
    TEST_ASSERT_MESSAGE(last_good_value > 1, "This value should not have been rollover");
    TEST_ASSERT_EQUAL_INT(0, Fibonacci_GetElement(first_overrun_element));
    TEST_ASSERT_EQUAL_INT(0, Fibonacci_GetElement(INT_MAX));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_element0_should_return1);
    RUN_TEST(test_element1_should_return1);
    RUN_TEST(test_element2_should_return2);
    RUN_TEST(test_part_of_the_sequence_style1);
    RUN_TEST(test_part_of_the_sequence_style2);
    RUN_TEST(test_negative_value_should_return0);
    RUN_TEST(test_overrun_value_should_return0);
    return UNITY_END();
}
