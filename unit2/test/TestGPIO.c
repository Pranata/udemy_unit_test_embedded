
#include "unity.h"
#include "GPIO.h"
#include "MK20DX256.h"

void test_SetPinAsOutput_should_ConfigurePinDirection(void)
{
    PORTC.PDDR = 0;

    TEST_ASSERT_EQUAL(GPIO_OK, GPIO_SetPinAsOutput(0));
    TEST_ASSERT_EQUAL_HEX32(BIT_TO_MASK(0), PORTC.PDDR);
    TEST_ASSERT_EQUAL(GPIO_OK, GPIO_SetPinAsOutput(11));
    TEST_ASSERT_EQUAL_HEX32(BIT_TO_MASK(0) | BIT_TO_MASK(11), PORTC.PDDR);
    TEST_ASSERT_EQUAL(GPIO_OK, GPIO_SetPinAsOutput(31));
    TEST_ASSERT_EQUAL_HEX32(BIT_TO_MASK(0) | BIT_TO_MASK(11) | BIT_TO_MASK(31), PORTC.PDDR);
}

void test_SetPinAsInput_should_ConfigurePinDirection(void)
{
    PORTC.PDDR = 0xFFFFFFFF;

    TEST_ASSERT_EQUAL(GPIO_OK, GPIO_SetPinAsInput(0));
    TEST_ASSERT_EQUAL_HEX32(~(BIT_TO_MASK(0)), PORTC.PDDR);
    TEST_ASSERT_EQUAL(GPIO_OK, GPIO_SetPinAsInput(11));
    TEST_ASSERT_EQUAL_HEX32(~(BIT_TO_MASK(0) | BIT_TO_MASK(11)), PORTC.PDDR);
    TEST_ASSERT_EQUAL(GPIO_OK, GPIO_SetPinAsInput(31));
    TEST_ASSERT_EQUAL_HEX32(~(BIT_TO_MASK(0) | BIT_TO_MASK(11) | BIT_TO_MASK(31)), PORTC.PDDR);
}

void test_SetPinAsOutput_should_NotUpdatePinDirection_when_PinIsNotValid (void)
{
    PORTC.PDDR = 0;

    TEST_ASSERT_NOT_EQUAL(GPIO_OK, GPIO_SetPinAsOutput(32));
    TEST_ASSERT_EQUAL_HEX32(0, PORTC.PDDR);
}

void test_SetPinAsInput_should_NotUpdatePinDirection_when_PinIsNotValid (void)
{
    PORTC.PDDR = 0xFFFFFFFF;

    TEST_ASSERT_NOT_EQUAL(GPIO_OK, GPIO_SetPinAsInput(32));
    TEST_ASSERT_EQUAL_HEX32(0xFFFFFFFF, PORTC.PDDR);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_SetPinAsOutput_should_ConfigurePinDirection);
    RUN_TEST(test_SetPinAsOutput_should_NotUpdatePinDirection_when_PinIsNotValid);
    RUN_TEST(test_SetPinAsInput_should_ConfigurePinDirection);
    RUN_TEST(test_SetPinAsInput_should_NotUpdatePinDirection_when_PinIsNotValid);
    return UNITY_END();
}
