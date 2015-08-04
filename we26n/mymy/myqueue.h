
#ifndef  _MY_QUEUE_H_
#define  _MY_QUEUE_H_


#define  BUF_MAX    256


/*
 */
typedef  struct _tag_ringbf_s
{
    uint16_t  head;
    uint16_t  tail;
    uint16_t  mask;
    uint8_t  buf[BUF_MAX];
} ringbf_s;


/* 只支持 单消费者, 单生产者模式.
 * 返回值 0 表示成功.
 */

int  ringbf_enqueue( ringbf_s * prb, int tlen, uint8_t * pdat );
int  ringbf_dequeue( ringbf_s * prb, int * plen, uint8_t * pdat );


/* 面向字节流. */
int  fifo_enqueue( ringbf_s * prb, int tlen, uint8_t * pdat );
int  fifo_dequeue( ringbf_s * prb, uint8_t * pdat );
int  fifo_deqall( ringbf_s * prb, int * plen, uint8_t * pdat );


#endif
