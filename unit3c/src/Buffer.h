#ifndef BUFFER_H
#define BUFFER_H

#define BUF_SIZE                             (1000 + 1)
#define BUF_ERROR_EMPTY                      0xF1
#define BUF_ERROR_FULL                       0xF2

int BufferPut(uint16_t val);
int BufferGet(uint16_t* p_val);
bool BufferIsFull(void);
bool BufferIsEmpty(void);
void BufferClear(void);

#endif //BUFFER_H
