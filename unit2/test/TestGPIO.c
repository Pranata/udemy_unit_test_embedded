
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

void test_SetPin_should_SetOutputHigh (void)
{
    PORTC.PSOR = 0;
    PORTC.PCOR = 0;

    TEST_ASSERT_EQUAL(GPIO_OK, GPIO_SetPin(0));
    TEST_ASSERT_EQUAL_HEX32(BIT_TO_MASK(0), PORTC.PSOR);
    TEST_ASSERT_EQUAL_HEX32(0, PORTC.PCOR);

    TEST_ASSERT_EQUAL(GPIO_OK, GPIO_SetPin(16));
    TEST_ASSERT_EQUAL_HEX32(BIT_TO_MASK(16), PORTC.PSOR);
    TEST_ASSERT_EQUAL_HEX32(0, PORTC.PCOR);

    TEST_ASSERT_EQUAL(GPIO_OK, GPIO_SetPin(31));
    TEST_ASSERT_EQUAL_HEX32(BIT_TO_MASK(31), PORTC.PSOR);
    TEST_ASSERT_EQUAL_HEX32(0, PORTC.PCOR);
}

void test_SetPin_should_ForceToOutput_when_ConfiguredAsInput(void)
{
    PORTC.PSOR = 0;
    PORTC.PCOR = 0;
    PORTC.PDDR = 0x50000000;

    TEST_ASSERT_EQUAL(GPIO_OK, GPIO_SetPin(0));
    TEST_ASSERT_EQUAL_HEX32(BIT_TO_MASK(0), PORTC.PSOR);
    TEST_ASSERT_EQUAL_HEX32(0, PORTC.PCOR);
    TEST_ASSERT_EQUAL_HEX32(0x50000001, PORTC.PDDR);
}

void test_SetPin_should_NotSetOutputs_when_PinIsNotValid(void)
{
    PORTC.PSOR = 0;
    PORTC.PCOR = 0;
    PORTC.PDDR = 0;

    TEST_ASSERT_NOT_EQUAL(GPIO_OK, GPIO_SetPin(32));
    TEST_ASSERT_EQUAL_HEX32(0, PORTC.PSOR);
    TEST_ASSERT_EQUAL_HEX32(0, PORTC.PCOR);
    TEST_ASSERT_EQUAL_HEX32(0, PORTC.PDDR);
}

void test_Init_should_ConfigurePinsToDefaults(void)
{
    PORTC.PDDR = 0;
    PORTC.PSOR = 0;
    PORTC.PCOR = 0;

    GPIO_Init();

    TEST_ASSERT_EQUAL_HEX32(0x1012A000, PORTC.PDDR);
    TEST_ASSERT_EQUAL_HEX32(0x10102000, PORTC.PSOR);
    TEST_ASSERT_EQUAL_HEX32(0x00028000, PORTC.PCOR);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_SetPinAsOutput_should_ConfigurePinDirection);
    RUN_TEST(test_SetPinAsOutput_should_NotUpdatePinDirection_when_PinIsNotValid);
    RUN_TEST(test_SetPinAsInput_should_ConfigurePinDirection);
    RUN_TEST(test_SetPinAsInput_should_NotUpdatePinDirection_when_PinIsNotValid);
    RUN_TEST(test_SetPin_should_SetOutputHigh);
    RUN_TEST(test_SetPin_should_ForceToOutput_when_ConfiguredAsInput);
    RUN_TEST(test_SetPin_should_NotSetOutputs_when_PinIsNotValid);
    RUN_TEST(test_Init_should_ConfigurePinsToDefaults);
    return UNITY_END();
}
