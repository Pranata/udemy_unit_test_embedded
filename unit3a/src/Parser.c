#include <stddef.h>
#include <stdint.h>

#include "Parser.h"
#include "MK20DX256.h"

#ifndef TEST
#define STATIC    static
#else
#define STATIC
#endif

#define CHECK_ASCII_HEX_CHAR(c)          \
    (                             \
    ((c >= '0') && (c <= '9')) || \
    ((c >= 'A') && (c <= 'Z')) || \
    ((c >= 'a') && (c <= 'f'))    \
    )


#define PARSER_BUF_MAX_LEN    23


static char    m_buf[PARSER_BUF_MAX_LEN];
static uint8_t m_buf_req_len;    // required length from packet length field value
static uint8_t m_buf_recv_len;   // save how many data received from packet
static uint8_t m_buf_idx;        // index number to traverser the m_buf


STATIC PARSER_STATE_T m_parser_state = PARSER_LOOKING_FOR_START;


char* Parser_AddChar(char new_char)
{
    if (new_char == '[')
    {
        m_parser_state   = PARSER_LOOKING_FOR_CMD;
        m_buf_req_len    = 0;
        m_buf_recv_len   = 0;
        m_buf_idx        = 0;
        m_buf[m_buf_idx] = new_char;
        return NULL;
    }

    switch(m_parser_state)
    {
        case PARSER_LOOKING_FOR_CMD:
            if ( (new_char >= 'A') && (new_char <= 'Z') )
            {
                ++m_buf_idx;
                m_buf[m_buf_idx] = new_char;
                m_parser_state   = PARSER_LOOKING_FOR_LEN;
            }
            else
            {
                m_parser_state = PARSER_LOOKING_FOR_START;
            }
            break;

        case PARSER_LOOKING_FOR_LEN:
            if (new_char == '0')
            {
                ++m_buf_idx;
                m_buf[m_buf_idx] = new_char;
                m_parser_state   = PARSER_LOOKING_FOR_END;
            }
            else if ((new_char >= '1') && (new_char <= '9'))
            {
                ++m_buf_idx;
                m_buf[m_buf_idx] = new_char;
                m_buf_req_len    = 2 * (new_char - '0'); // ASCII hex niblle requires two bytes to represent one byte
                m_parser_state   = PARSER_LOOKING_FOR_DATA;
            }
            else
            {
                m_parser_state = PARSER_LOOKING_FOR_START;
            }
            
            break;

        case PARSER_LOOKING_FOR_DATA:
            if (CHECK_ASCII_HEX_CHAR(new_char))
            {
                ++m_buf_idx;
                ++m_buf_recv_len;
                m_buf[m_buf_idx] = new_char;

                if (m_buf_recv_len < m_buf_req_len)
                    m_parser_state = PARSER_LOOKING_FOR_DATA;
                else
                    m_parser_state = PARSER_LOOKING_FOR_END;
            }
            else
            {
                m_parser_state = PARSER_LOOKING_FOR_START;
            }
            
            break;
        
        case PARSER_LOOKING_FOR_END:
            if (new_char == ']')
            {
                ++m_buf_idx;
                m_buf[m_buf_idx    ] = new_char;
                m_buf[m_buf_idx + 1] = '\0';
                m_parser_state = PARSER_LOOKING_FOR_START;
                
                return m_buf;
            }
            else
            {
                m_parser_state = PARSER_LOOKING_FOR_START;
                return NULL;
            }
            break;

        default:
            break;
    }

    return NULL;
}