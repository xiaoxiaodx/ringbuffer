#include "ringBuffer.h"
#include <iostream>
#include <algorithm>
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
int tmp_id=1;

 int testcount = 100000;
struct student_info
{
    unsigned int stu_id;
};

int preid = 0;
void print_student_info(const student_info *stu_info)
{
    if (NULL == stu_info)
        return;
   
    int curid =  stu_info->stu_id;
    if(curid - preid != 1){
        printf("consumer exit id:%d\n", stu_info->stu_id);
        exit(0);
    }
    if(curid == testcount-1)
        printf("consumer last id:%d\n", stu_info->stu_id);

    printf("consumer id:%d\n", stu_info->stu_id);

 
    preid =  stu_info->stu_id;
 
}


student_info *get_student_info(time_t timer)
{
    student_info *stu_info = (student_info *)malloc(sizeof(student_info));
    if (!stu_info)
    {
        fprintf(stderr, "Failed to malloc memory.\n");
        return NULL;
    }
    srand(timer);
    unsigned int  id = tmp_id;
    if(tmp_id > testcount){
        printf("producer exit id:%d\n", id);
        exit(0);
    }
        
    stu_info->stu_id = id;
 
    printf("producer id:%d\t", id);

    return stu_info;
}

void *consumer_proc(void *arg)
{
    RingBuffer *queue = (RingBuffer *)arg;
    student_info stu_info;
    while (1)
    {
        usleep(20);
        unsigned int len = queue->Get((unsigned char *)&stu_info, sizeof(student_info));
        if (len > 0)
        {
    
            print_student_info(&stu_info);
     
        }else{
            //printf("----------------------未获取数据--------------------\n\n");
        }
    }
    return (void *)queue;
}

void *producer_proc(void *arg)
{
    time_t cur_time;
    RingBuffer *queue = (RingBuffer *)arg;
    while (1)
    {
        time(&cur_time);
        srand(cur_time);
        int seed = rand() % 11111;

        student_info *stu_info = get_student_info(cur_time + seed);
        if(stu_info == NULL){continue;}
        unsigned int putlen = queue->Put((unsigned char *)stu_info, sizeof(student_info));
        if(putlen > 0)
            tmp_id++;
        else
            printf("RingBuffer full !!!! %u \n", queue->GetDataLen());
        free(stu_info);
       // printf("RingBuffer length: %u\n", queue->GetDataLen());
        //printf("******************************************\n");
        usleep(10);
    }
    return (void *)queue;
}

int main()
{
    RingBuffer *ringBuffer = new RingBuffer(1024);
    if (!ringBuffer->Initialize())
    {
        return -1;
    }

    pthread_t consumer_tid, producer_tid;

    
    if (0 != pthread_create(&producer_tid, NULL, producer_proc, (void *)ringBuffer))
    {
        fprintf(stderr, "Failed to create consumer thread.errno:%u, reason:%s\n",
                errno, strerror(errno));
        return -1;
    }

    if (0 != pthread_create(&consumer_tid, NULL, consumer_proc, (void *)ringBuffer))
    {
        fprintf(stderr, "Failed to create consumer thread.errno:%u, reason:%s\n",
                errno, strerror(errno));
        return -1;
    }

    pthread_join(producer_tid, NULL);
    pthread_join(consumer_tid, NULL);

    return 0;
}
