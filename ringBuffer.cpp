#include "ringBuffer.h"
#include "math.h"
#include <cstring>
#include <algorithm>


RingBuffer::RingBuffer(int nSize)
    : m_pBuffer(NULL), m_nSize(nSize), m_w_index(0), m_r_index(0)
{
    
}

RingBuffer::~RingBuffer()
{
    if (NULL != m_pBuffer)
    {
        delete[] m_pBuffer;
        m_pBuffer = NULL;
    }
}
unsigned int RingBuffer::GetIn()
{
    return m_w_index;
}
unsigned int RingBuffer::GetOut()
{
    return m_r_index;
}
bool RingBuffer::Initialize()
{
    m_pBuffer = new unsigned char[m_nSize];
    if (!m_pBuffer)
    {
        return false;
    }

    m_w_index = m_r_index = 0;

    return true;
}



unsigned int RingBuffer::Put(const unsigned char *buffer, unsigned int len)
{


    unsigned int cur_r = m_r_index;
    unsigned int datalen  = m_w_index - cur_r;
    //已经满了
    if(datalen >= m_nSize)
        return 0;
   
    //索引在数组的位置
    unsigned int t_w  = m_w_index   & (m_nSize - 1);
    unsigned int t_r = cur_r & (m_nSize - 1);


    if(t_w >= t_r){
        
        if(len <= (m_nSize-t_w)){
            memcpy(m_pBuffer + t_w, buffer, len);
        }else{
            
            memcpy(m_pBuffer + t_w, buffer, (m_nSize-t_w));
            memcpy(m_pBuffer , buffer, len - (m_nSize-t_w));
        }
    }else{
         memcpy(m_pBuffer + t_w , buffer, len);
    }
    m_w_index += len;
    return len;
}


unsigned int RingBuffer::Get(unsigned char *buffer, unsigned int len)
{
    unsigned int cur_w = m_w_index;
    unsigned int datalen  = cur_w - m_r_index;
    //空了或小于
    if(datalen < len || datalen ==0 )
        return 0;
   
    //索引在数组的位置
    unsigned int t_w  = cur_w & (m_nSize - 1);
    unsigned int t_r = m_r_index & (m_nSize - 1);


    if(t_w > t_r){
        
        memcpy(buffer,m_pBuffer + t_r, len);

    }else{

 
            if(len <=  (m_nSize - t_r))
                memcpy(buffer,m_pBuffer + t_r, len);
            else{
                memcpy(buffer,m_pBuffer + t_r, (m_nSize - t_r));
                memcpy(buffer+(m_nSize - t_r),m_pBuffer, len - (m_nSize - t_r));
            }

        
            
    }
    m_r_index += len;
    return len;
}
