
#include "unity.h"
#include "Parser.h"
#include "MK20DX256.h"


extern PARSER_STATE_T m_parser_state;


void insert_valid_minimal_packet(char cmd, const char* full)
{
    TEST_ASSERT_NULL(Parser_AddChar('['));
    TEST_ASSERT_NULL(Parser_AddChar(cmd));
    TEST_ASSERT_NULL(Parser_AddChar('0'));
    TEST_ASSERT_EQUAL_STRING(full, Parser_AddChar(']'));
}


void test_Parser_AddChar_should_StartLookingForCmdOnLeftBracket(void)
{
    m_parser_state = PARSER_LOOKING_FOR_START;

    TEST_ASSERT_NULL(Parser_AddChar('['));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_CMD, m_parser_state);
}


void test_Parser_AddChar_should_RejectNonBracketCharsWhileSeekingStart(void)
{
    m_parser_state = PARSER_LOOKING_FOR_START;

    TEST_ASSERT_NULL(Parser_AddChar('|'));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_START, m_parser_state);

    TEST_ASSERT_NULL(Parser_AddChar('9'));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_START, m_parser_state);

    TEST_ASSERT_NULL(Parser_AddChar('A'));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_START, m_parser_state);
}


void test_Parser_AddChar_should_AcceptAValidCommandChar(void)
{
    m_parser_state = PARSER_LOOKING_FOR_CMD;
    TEST_ASSERT_NULL(Parser_AddChar('A'));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_LEN, m_parser_state);

    m_parser_state = PARSER_LOOKING_FOR_CMD;
    TEST_ASSERT_NULL(Parser_AddChar('M'));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_LEN, m_parser_state);

    m_parser_state = PARSER_LOOKING_FOR_CMD;
    TEST_ASSERT_NULL(Parser_AddChar('Z'));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_LEN, m_parser_state);
}


void test_Parser_AddChar_should_RejectInvalidCommandChar(void)
{
    m_parser_state = PARSER_LOOKING_FOR_CMD;
    TEST_ASSERT_NULL(Parser_AddChar('@'));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_START, m_parser_state);

    m_parser_state = PARSER_LOOKING_FOR_CMD;
    TEST_ASSERT_NULL(Parser_AddChar('a'));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_START, m_parser_state);

    m_parser_state = PARSER_LOOKING_FOR_CMD;
    TEST_ASSERT_NULL(Parser_AddChar('\\'));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_START, m_parser_state);
}


void test_Parser_AddChar_should_AcceptALengthOfZero(void)
{
    m_parser_state = PARSER_LOOKING_FOR_LEN;
    TEST_ASSERT_NULL(Parser_AddChar('0'));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_END, m_parser_state);
}


void test_Parser_AddChar_should_RejectInvalidLengths(void)
{
    m_parser_state = PARSER_LOOKING_FOR_LEN;
    TEST_ASSERT_NULL(Parser_AddChar(':'));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_START, m_parser_state);
}


void test_Parser_AddChar_should_HandleValidPacketWithNoData(void)
{
    insert_valid_minimal_packet('A', "[A0]");
}


void test_Parser_AddChar_should_HandleBackToBackValidPackets(void)
{
    insert_valid_minimal_packet('A', "[A0]");
    insert_valid_minimal_packet('M', "[M0]");
    insert_valid_minimal_packet('Z', "[Z0]");
}


void test_Parser_AddChar_should_IgnoreBadStartCharacter(void)
{
    TEST_ASSERT_NULL(Parser_AddChar('?'));
    insert_valid_minimal_packet('D', "[D0]");

    TEST_ASSERT_NULL(Parser_AddChar('a'));
    TEST_ASSERT_NULL(Parser_AddChar(']'));
    insert_valid_minimal_packet('X', "[X0]");
}


void test_Parser_AddChar_should_IgnoreBadCommandCharacter(void)
{
    TEST_ASSERT_NULL(Parser_AddChar('['));
    TEST_ASSERT_NULL(Parser_AddChar('a'));
    insert_valid_minimal_packet('X', "[X0]");

    TEST_ASSERT_NULL(Parser_AddChar('['));
    TEST_ASSERT_NULL(Parser_AddChar('z'));
    TEST_ASSERT_NULL(Parser_AddChar('0'));
    TEST_ASSERT_NULL(Parser_AddChar(']'));
    insert_valid_minimal_packet('S', "[S0]");
}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_Parser_AddChar_should_StartLookingForCmdOnLeftBracket);
    RUN_TEST(test_Parser_AddChar_should_RejectNonBracketCharsWhileSeekingStart);
    RUN_TEST(test_Parser_AddChar_should_AcceptAValidCommandChar);
    RUN_TEST(test_Parser_AddChar_should_RejectInvalidCommandChar);
    RUN_TEST(test_Parser_AddChar_should_AcceptALengthOfZero);
    RUN_TEST(test_Parser_AddChar_should_RejectInvalidLengths);
    RUN_TEST(test_Parser_AddChar_should_HandleValidPacketWithNoData);
    RUN_TEST(test_Parser_AddChar_should_HandleBackToBackValidPackets);
    RUN_TEST(test_Parser_AddChar_should_IgnoreBadStartCharacter);
    RUN_TEST(test_Parser_AddChar_should_IgnoreBadCommandCharacter);
    return UNITY_END();
}
