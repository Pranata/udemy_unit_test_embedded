#include <stddef.h>

#include "Parser.h"
#include "MK20DX256.h"

#ifndef TEST
#define STATIC    static
#else
#define STATIC
#endif

#define PARSER_BUF_MAX_LEN    23

static char m_buf[PARSER_BUF_MAX_LEN];

STATIC PARSER_STATE_T m_parser_state = PARSER_LOOKING_FOR_START;


char* Parser_AddChar(char new_char)
{
    if (new_char == '[')
    {
        m_parser_state = PARSER_LOOKING_FOR_CMD;
        m_buf[0] = new_char;
        return NULL;
    }

    switch(m_parser_state)
    {
        case PARSER_LOOKING_FOR_CMD:
            if ( (new_char >= 'A') && (new_char <= 'Z') )
            {
                m_parser_state = PARSER_LOOKING_FOR_LEN;
                m_buf[1] = new_char;
            }
            else
            {
                m_parser_state = PARSER_LOOKING_FOR_START;
            }
            break;

        case PARSER_LOOKING_FOR_LEN:
            if (new_char == '0')
            {
                m_parser_state = PARSER_LOOKING_FOR_END;
                m_buf[2] = new_char;
            }
            else
            {
                m_parser_state = PARSER_LOOKING_FOR_START;
            }
            break;

        case PARSER_LOOKING_FOR_END:
            if (new_char == ']')
            {
                m_parser_state = PARSER_LOOKING_FOR_START;
                m_buf[3] = new_char;
                m_buf[4] = '\0';
                return m_buf;
            }
            break;

        default:
            break;
    }

    return NULL;
}