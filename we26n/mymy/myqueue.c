
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "myqueue.h"




/* ÿ�����ݰ��ĸ�ʽ, һ�ֽڵ� len ���� �����ֽڵ�����, �����ܳ����� len + 1 �ֽ�. */
int  ringbf_enqueue( ringbf_s * prb, int tlen, uint8_t * pdat )
{
	int  i;
	int  frees;
	uint16_t  temp;
	
	frees = prb->mask + prb->head - prb->tail;
	if ( tlen + 1 > frees )
	{
		return 1;
	}
	
	temp = prb->tail;
	
	prb->buf[temp & prb->mask] = (uint8_t)tlen;
	temp = temp + 1;
	
	for ( i=0; i<tlen; i++ )
	{
		prb->buf[temp & prb->mask] = pdat[i];
		temp = temp + 1;		
	}
	
	prb->tail = temp;
	return 0;
}


int  ringbf_dequeue( ringbf_s * prb, int * plen, uint8_t * pdat )
{
	int  i;
	int  entrys;
	uint16_t  temp;
	
	entrys = prb->tail - prb->head;
	if ( entrys == 0 )
	{
		/*  */
		return 1;
	}
	
	temp = prb->head;
	
	entrys = (int)(prb->buf[temp & prb->mask]);
	temp = temp + 1;

	if ( entrys > *plen )
	{
		return 2;
	}
	
	for ( i=0; i<entrys; i++ )
	{
		pdat[i] = prb->buf[temp & prb->mask];
		temp = temp + 1;
	}
	
	prb->head = temp;
	*plen = entrys;
	return 0;
}



/* �����治ͬ��, ���� len �ŵ� ring ��, �������ֽ���. */
int  fifo_enqueue( ringbf_s * prb, int tlen, uint8_t * pdat )
{
	int  i;
	int  frees;
	uint16_t  temp;
	
	frees = prb->mask + prb->head - prb->tail;
	if ( tlen > frees )
	{
		return 1;
	}
	
	temp = prb->tail;
	
	for ( i=0; i<tlen; i++ )
	{
		prb->buf[temp & prb->mask] = pdat[i];
		temp = temp + 1;		
	}
	
	prb->tail = temp;
	return 0;
}



/* ÿ�ε��ֽ����? */
int  fifo_dequeue( ringbf_s * prb, uint8_t * pdat )
{
	int  entrys;
	uint16_t  temp;
	
	entrys = prb->tail - prb->head;
	if ( entrys == 0 )
	{
		/*  */
		return 1;
	}
	
	temp = prb->head;
	
	pdat[0] = prb->buf[temp & prb->mask];
	temp = temp + 1;

	prb->head = temp;
	return 0;
}



int  fifo_deqall( ringbf_s * prb, int * plen, uint8_t * pdat )
{
	int  i;
	int  entrys;
	uint16_t  temp;
	
	entrys = prb->tail - prb->head;
	if ( entrys == 0 )
	{
		/*  */
		return 1;
	}
	
	temp = prb->head;
	
	for ( i=0; i<entrys; i++ )
	{
		pdat[i] = prb->buf[temp & prb->mask];
		temp = temp + 1;
	}
	
	prb->head = temp;
	*plen = entrys;
	return 0;
}


