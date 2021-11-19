#ifndef RINGBUFFER_H
#define RINGBUFFER_H
#include <stdint.h>
class RingBuffer
{
public:
    RingBuffer(int nSize);
    ~RingBuffer();

    bool Initialize();

    unsigned int Put(const uint8_t *pBuffer, unsigned int nLen);
    unsigned int Get(uint8_t *pBuffer, unsigned int nLen);

    unsigned int GetIn();
    unsigned int GetOut();

    inline void Clean() { m_w_index = m_r_index = 0; }
    inline unsigned int GetDataLen() const { return m_w_index - m_r_index; }

private:
    inline bool is_power_of_2(unsigned long n) { return (n != 0 && ((n & (n - 1)) == 0)); };
    inline unsigned long roundup_power_of_two(unsigned long val);

private:
    uint8_t *m_pBuffer; 
    unsigned int m_nSize;     
    unsigned int m_w_index;      
    unsigned int m_r_index;     
};

#endif