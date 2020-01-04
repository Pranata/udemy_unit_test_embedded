
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


void insert_valid_packet(char cmd, char length, const char* p_data, size_t data_len)
{
    size_t ii;
    char* p_temp = p_data;

    TEST_ASSERT_NULL(Parser_AddChar('['));
    TEST_ASSERT_NULL(Parser_AddChar(cmd));
    TEST_ASSERT_NULL(Parser_AddChar(length));
    
    for(ii = 0; ii < (data_len - 1); ii++) //do not insert the NULL terminated string
    {
        TEST_ASSERT_NULL(Parser_AddChar(*p_temp++));
    }
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


void test_Parser_AddChar_should_AcceptValidLengths(void)
{
    m_parser_state = PARSER_LOOKING_FOR_LEN;
    TEST_ASSERT_NULL(Parser_AddChar('1'));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_DATA, m_parser_state);

    m_parser_state = PARSER_LOOKING_FOR_LEN;
    TEST_ASSERT_NULL(Parser_AddChar('9'));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_DATA, m_parser_state);

    m_parser_state = PARSER_LOOKING_FOR_LEN;
    TEST_ASSERT_NULL(Parser_AddChar('5'));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_DATA, m_parser_state);
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

    insert_valid_packet('A', '1', "00", sizeof("00"));
    TEST_ASSERT_EQUAL_STRING("[A100]", Parser_AddChar(']'));

    insert_valid_packet('M', '5', "a1a2a3a4a5", sizeof("a1a2a3a4a5"));
    TEST_ASSERT_EQUAL_STRING("[M5a1a2a3a4a5]", Parser_AddChar(']'));

    insert_valid_packet('Z', '9', "010203040506070809", sizeof("010203040506070809"));
    TEST_ASSERT_EQUAL_STRING("[Z9010203040506070809]", Parser_AddChar(']'));
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


void test_Parser_AddChar_should_HandleValidPacketsWithOneByteData(void)
{
    m_parser_state = PARSER_LOOKING_FOR_START;
    TEST_ASSERT_NULL(Parser_AddChar('['));
    TEST_ASSERT_NULL(Parser_AddChar('A'));
    TEST_ASSERT_NULL(Parser_AddChar('1'));
    TEST_ASSERT_NULL(Parser_AddChar('0'));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_DATA, m_parser_state);

    m_parser_state = PARSER_LOOKING_FOR_START;
    TEST_ASSERT_NULL(Parser_AddChar('['));
    TEST_ASSERT_NULL(Parser_AddChar('A'));
    TEST_ASSERT_NULL(Parser_AddChar('1'));
    TEST_ASSERT_NULL(Parser_AddChar('0'));
    TEST_ASSERT_NULL(Parser_AddChar('0'));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_END, m_parser_state);

    m_parser_state = PARSER_LOOKING_FOR_START;
    TEST_ASSERT_NULL(Parser_AddChar('['));
    TEST_ASSERT_NULL(Parser_AddChar('A'));
    TEST_ASSERT_NULL(Parser_AddChar('1'));
    TEST_ASSERT_NULL(Parser_AddChar('0'));
    TEST_ASSERT_NULL(Parser_AddChar('0'));
    TEST_ASSERT_EQUAL_STRING("[A100]", Parser_AddChar(']'));
}


void test_Parser_AddChar_should_HandleValidPacketWithArbitraryDataLength(void)
{
    m_parser_state = PARSER_LOOKING_FOR_START;
    insert_valid_packet('A', '1', "00", sizeof("00"));
    TEST_ASSERT_EQUAL_STRING("[A100]", Parser_AddChar(']'));

    insert_valid_packet('M', '5', "a1a2a3a4a5", sizeof("a1a2a3a4a5"));
    TEST_ASSERT_EQUAL_STRING("[M5a1a2a3a4a5]", Parser_AddChar(']'));

    insert_valid_packet('Z', '9', "010203040506070809", sizeof("010203040506070809"));
    TEST_ASSERT_EQUAL_STRING("[Z9010203040506070809]", Parser_AddChar(']'));
}


void test_Parser_AddChar_should_BackToStart_when_ReceiveInvalidEndCharacter(void)
{
    m_parser_state = PARSER_LOOKING_FOR_START;
    insert_valid_packet('A', '1', "00", sizeof("00"));
    TEST_ASSERT_NULL(Parser_AddChar('|'));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_START, m_parser_state);
}


void test_Parser_AddChar_should_BackToStart_when_ReceiveInvalidDataCharacter(void)
{
    m_parser_state = PARSER_LOOKING_FOR_START;
    TEST_ASSERT_NULL(Parser_AddChar('['));
    TEST_ASSERT_NULL(Parser_AddChar('A'));
    TEST_ASSERT_NULL(Parser_AddChar('1'));
    TEST_ASSERT_NULL(Parser_AddChar(':'));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_START, m_parser_state);

    m_parser_state = PARSER_LOOKING_FOR_START;
    TEST_ASSERT_NULL(Parser_AddChar('['));
    TEST_ASSERT_NULL(Parser_AddChar('A'));
    TEST_ASSERT_NULL(Parser_AddChar('1'));
    TEST_ASSERT_NULL(Parser_AddChar('A'));
    TEST_ASSERT_NULL(Parser_AddChar('\\'));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_START, m_parser_state);
}


void test_Parser_AddChar_should_BackToLookForCommand_and_ReturnValidPacket_when_ReceiveLeftBracket(void)
{
    m_parser_state = PARSER_LOOKING_FOR_START;
    TEST_ASSERT_NULL(Parser_AddChar('['));
    TEST_ASSERT_NULL(Parser_AddChar('A'));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_LEN, m_parser_state);
    TEST_ASSERT_NULL(Parser_AddChar('['));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_CMD, m_parser_state);

    m_parser_state = PARSER_LOOKING_FOR_START;
    TEST_ASSERT_NULL(Parser_AddChar('['));
    TEST_ASSERT_NULL(Parser_AddChar('Z'));
    TEST_ASSERT_NULL(Parser_AddChar('1'));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_DATA, m_parser_state);
    TEST_ASSERT_NULL(Parser_AddChar('['));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_CMD, m_parser_state);

    m_parser_state = PARSER_LOOKING_FOR_START;
    TEST_ASSERT_NULL(Parser_AddChar('['));
    TEST_ASSERT_NULL(Parser_AddChar('M'));
    TEST_ASSERT_NULL(Parser_AddChar('1'));
    TEST_ASSERT_NULL(Parser_AddChar('0'));
    TEST_ASSERT_NULL(Parser_AddChar('0'));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_END, m_parser_state);
    TEST_ASSERT_NULL(Parser_AddChar('['));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_CMD, m_parser_state);

    m_parser_state = PARSER_LOOKING_FOR_START;
    TEST_ASSERT_NULL(Parser_AddChar('['));
    TEST_ASSERT_NULL(Parser_AddChar('A'));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_LEN, m_parser_state);
    insert_valid_packet('M', '5', "a1a2a3a4a5", sizeof("a1a2a3a4a5"));
    TEST_ASSERT_EQUAL_STRING("[M5a1a2a3a4a5]", Parser_AddChar(']'));

    m_parser_state = PARSER_LOOKING_FOR_START;
    TEST_ASSERT_NULL(Parser_AddChar('['));
    TEST_ASSERT_NULL(Parser_AddChar('Z'));
    TEST_ASSERT_NULL(Parser_AddChar('1'));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_DATA, m_parser_state);
    insert_valid_packet('M', '5', "a1a2a3a4a5", sizeof("a1a2a3a4a5"));
    TEST_ASSERT_EQUAL_STRING("[M5a1a2a3a4a5]", Parser_AddChar(']'));

    m_parser_state = PARSER_LOOKING_FOR_START;
    TEST_ASSERT_NULL(Parser_AddChar('['));
    TEST_ASSERT_NULL(Parser_AddChar('M'));
    TEST_ASSERT_NULL(Parser_AddChar('1'));
    TEST_ASSERT_NULL(Parser_AddChar('0'));
    TEST_ASSERT_NULL(Parser_AddChar('0'));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_END, m_parser_state);
    insert_valid_packet('M', '5', "a1a2a3a4a5", sizeof("a1a2a3a4a5"));
    TEST_ASSERT_EQUAL_STRING("[M5a1a2a3a4a5]", Parser_AddChar(']'));
}


void test_Parser_AddChar_should_HandleUnpairedNibbles(void)
{
    TEST_ASSERT_NULL(Parser_AddChar('['));
    TEST_ASSERT_NULL(Parser_AddChar('M'));
    TEST_ASSERT_NULL(Parser_AddChar('1'));
    TEST_ASSERT_NULL(Parser_AddChar('0'));
    TEST_ASSERT_NULL(Parser_AddChar(']'));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_START, m_parser_state);

    TEST_ASSERT_NULL(Parser_AddChar('['));
    TEST_ASSERT_NULL(Parser_AddChar('A'));
    TEST_ASSERT_NULL(Parser_AddChar('2'));
    TEST_ASSERT_NULL(Parser_AddChar('a'));
    TEST_ASSERT_NULL(Parser_AddChar('1'));
    TEST_ASSERT_NULL(Parser_AddChar(']'));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_START, m_parser_state);

    TEST_ASSERT_NULL(Parser_AddChar('['));
    TEST_ASSERT_NULL(Parser_AddChar('A'));
    TEST_ASSERT_NULL(Parser_AddChar('2'));
    TEST_ASSERT_NULL(Parser_AddChar('a'));
    TEST_ASSERT_NULL(Parser_AddChar('1'));
    TEST_ASSERT_NULL(Parser_AddChar('b'));
    TEST_ASSERT_NULL(Parser_AddChar(']'));
    TEST_ASSERT_EQUAL(PARSER_LOOKING_FOR_START, m_parser_state);
}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_Parser_AddChar_should_StartLookingForCmdOnLeftBracket);
    RUN_TEST(test_Parser_AddChar_should_RejectNonBracketCharsWhileSeekingStart);
    RUN_TEST(test_Parser_AddChar_should_AcceptAValidCommandChar);
    RUN_TEST(test_Parser_AddChar_should_RejectInvalidCommandChar);
    RUN_TEST(test_Parser_AddChar_should_AcceptALengthOfZero);
    RUN_TEST(test_Parser_AddChar_should_RejectInvalidLengths);
    RUN_TEST(test_Parser_AddChar_should_AcceptValidLengths);
    RUN_TEST(test_Parser_AddChar_should_HandleValidPacketWithNoData);
    RUN_TEST(test_Parser_AddChar_should_HandleBackToBackValidPackets);
    RUN_TEST(test_Parser_AddChar_should_IgnoreBadStartCharacter);
    RUN_TEST(test_Parser_AddChar_should_IgnoreBadCommandCharacter);
    RUN_TEST(test_Parser_AddChar_should_HandleValidPacketsWithOneByteData);
    RUN_TEST(test_Parser_AddChar_should_HandleValidPacketWithArbitraryDataLength);
    RUN_TEST(test_Parser_AddChar_should_BackToStart_when_ReceiveInvalidEndCharacter);
    RUN_TEST(test_Parser_AddChar_should_BackToStart_when_ReceiveInvalidDataCharacter);
    RUN_TEST(test_Parser_AddChar_should_BackToLookForCommand_and_ReturnValidPacket_when_ReceiveLeftBracket);
    RUN_TEST(test_Parser_AddChar_should_HandleUnpairedNibbles);
    return UNITY_END();
}
