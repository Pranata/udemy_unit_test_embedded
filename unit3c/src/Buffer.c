#include <stdint.h>
#include "Buffer.h"
#include "MK20DX256.h"

uint16_t m_buffer[BUF_SIZE];
uint16_t m_buffer_read = 0, m_buffer_write = 0;


int BufferPut(uint16_t val)
{
    uint16_t next_write = (m_buffer_write + 1) % BUF_SIZE;

    if (next_write == m_buffer_read)
        return BUF_ERROR_FULL;
    
    m_buffer[m_buffer_write] = val;
    m_buffer_write = (m_buffer_write + 1) % BUF_SIZE;
    return 0;
}


int BufferGet(uint16_t* p_val)
{
    if (m_buffer_write == m_buffer_read)
        return BUF_ERROR_EMPTY;
    
    *p_val = m_buffer[m_buffer_read];
    m_buffer_read = (m_buffer_read + 1) % BUF_SIZE;
    
    return 0;
}