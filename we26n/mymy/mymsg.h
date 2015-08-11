
#ifndef  __MY_MMSG_H__
#define  __MY_MMSG_H__


int  msg_send_to_pend( int tidx, int tlen, uint8_t * pdat );

/* only called, in INT priority:: (1<<__NVIC_PRIO_BITS) - 1 */
int  msg_send_to_host( int tidx, int tlen, uint8_t * pdat );

#endif
