
#include <stdint.h>
#include "stm32f0xx_usart.h"

#include "myuart.h"
#include "myqueue.h"
#include "mymsg.h"
#include "mysvch.h"


/*
u0, UART1, --> 7620   --> PA9,10
u1, UART2, --> rf433  --> PA2,3    --> 9600
u2, UART3, --> BLE40  --> PB10,11
u3, UART4, --> zigbee --> PA0,1    --> 57600
*/


static ringbf_s  u0_send_ring = { 0, 0, BUF_MAX-1, { 0 } };
static ringbf_s  u1_send_ring = { 0, 0, BUF_MAX-1, { 0 } };
static ringbf_s  u2_send_ring = { 0, 0, BUF_MAX-1, { 0 } };
static ringbf_s  u3_send_ring = { 0, 0, BUF_MAX-1, { 0 } };


int  my_uart_init( void )
{
	USART_InitTypeDef USART_InitStructure;
	
	/**/
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  /* Prepare all uart to receive a data packet */
  USART_Init( USART1, &USART_InitStructure);

	/**/
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  /* Prepare all uart to receive a data packet */
  USART_Init( USART2, &USART_InitStructure);
	
	/**/
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  /* Prepare all uart to receive a data packet */
  USART_Init( USART3, &USART_InitStructure);
	
	/**/
  USART_InitStructure.USART_BaudRate = 57600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  /* Prepare all uart to receive a data packet */
  USART_Init( USART4, &USART_InitStructure);
	
  /*  */
  USART_ITConfig( USART1, USART_IT_RXNE, ENABLE );
  USART_Cmd( USART1, ENABLE );
	
  USART_ITConfig( USART2, USART_IT_RXNE, ENABLE );
  USART_Cmd( USART2, ENABLE );

  USART_ITConfig( USART3, USART_IT_RXNE, ENABLE );
  USART_Cmd( USART3, ENABLE );
	
  USART_ITConfig( USART4, USART_IT_RXNE, ENABLE );
  USART_Cmd( USART4, ENABLE );
  return 0;
}



int  my_uart_reinit( USART_TypeDef * USARTx, uint32_t baud )
{
	USART_InitTypeDef USART_InitStructure;
	
	USART_Cmd( USARTx, DISABLE );
	
	/**/
  USART_InitStructure.USART_BaudRate = baud;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  /* Prepare all uart to receive a data packet */
  USART_Init( USARTx, &USART_InitStructure);

	
  USART_ITConfig( USARTx, USART_IT_RXNE, ENABLE );
	USART_Cmd( USARTx, ENABLE );
	return 0;
}



/* call by pendsv handler */
int  my_uart_baudrate( int tidx, uint32_t baud )
{
	if ( (tidx < 0) || (tidx > 3) )
	{
		return 1;
	}
	
	testsvc( tidx+4, (intptr_t)baud, 0 );
	return 0;
}



/* call by pendsv handler */
int  my_uart_send( int tidx, int tlen, uint8_t * pdat )
{
	switch ( tidx )
	{
	case 0:
		fifo_enqueue( &u0_send_ring, tlen, pdat );
		testsvc( 0, tlen, (intptr_t)pdat );
	  break;
	
	case 1:
		fifo_enqueue( &u1_send_ring, tlen, pdat );
		testsvc( 1, tlen, (intptr_t)pdat );
	  break;

	case 2:
		fifo_enqueue( &u2_send_ring, tlen, pdat );
		testsvc( 2, tlen, (intptr_t)pdat );
	  break;
	
	case 3:
		fifo_enqueue( &u3_send_ring, tlen, pdat );
		testsvc( 3, tlen, (intptr_t)pdat );
	  break;
	}
	
	return 0;
}



void  USART1_IRQHandler(void)
{
	int  iret;
  uint8_t  temp;
  
	if((USART1->ISR & USART_ISR_ORE) == USART_ISR_ORE)
	{
		USART_ClearFlag( USART1, USART_FLAG_ORE );
	}
	
  if((USART1->ISR & USART_ISR_RXNE) == USART_ISR_RXNE)
  {
		temp = (uint8_t)USART_ReceiveData( USART1 );
		msg_send_to_pend( 0, 1, &temp );
  }
	
	if( USART_GetITStatus(USART1, USART_IT_TXE ) != RESET )
	{
		iret = fifo_dequeue( &u0_send_ring, &temp );
		if ( 0 == iret )
		{
			USART_SendData( USART1, temp );
		}
		else
		{
			USART_ITConfig( USART1, USART_IT_TXE, DISABLE );
		}
	}
}



void  USART2_IRQHandler( void )
{
	int  iret;
  uint8_t  temp;
  
	if((USART2->ISR & USART_ISR_ORE) == USART_ISR_ORE)
	{
		USART_ClearFlag( USART2, USART_FLAG_ORE );
	}
	
  if((USART2->ISR & USART_ISR_RXNE) == USART_ISR_RXNE)
  {
		temp = (uint8_t)USART_ReceiveData( USART2 );
		msg_send_to_pend( 1, 1, &temp );
  }
	
	if( USART_GetITStatus(USART2, USART_IT_TXE ) != RESET )
	{
		iret = fifo_dequeue( &u1_send_ring, &temp );
		if ( 0 == iret )
		{
			USART_SendData( USART2, temp );
		}
		else
		{
			USART_ITConfig( USART2, USART_IT_TXE, DISABLE );
		}
	}
}



void  USART3_4_IRQHandler( void )
{
	int  iret;
  uint8_t  temp;
  
	if((USART4->ISR & USART_ISR_ORE) == USART_ISR_ORE)
	{
		USART_ClearFlag( USART4, USART_FLAG_ORE );
	}
	
  if((USART4->ISR & USART_ISR_RXNE) == USART_ISR_RXNE)
  {
		temp = (uint8_t)USART_ReceiveData( USART4 );
		msg_send_to_pend( 3, 1, &temp );
  }
	
	if( USART_GetITStatus(USART4, USART_IT_TXE ) != RESET )
	{
		iret = fifo_dequeue( &u3_send_ring, &temp );
		if ( 0 == iret )
		{
			USART_SendData( USART4, temp );
		}
		else
		{
			USART_ITConfig( USART4, USART_IT_TXE, DISABLE );
		}
	}
	
	
	/**/
 	if((USART3->ISR & USART_ISR_ORE) == USART_ISR_ORE)
	{
		USART_ClearFlag( USART3, USART_FLAG_ORE );
	}
	
  if((USART3->ISR & USART_ISR_RXNE) == USART_ISR_RXNE)
  {
		temp = (uint8_t)USART_ReceiveData( USART3 );
		msg_send_to_pend( 2, 1, &temp );
  }
	
	if( USART_GetITStatus(USART3, USART_IT_TXE ) != RESET )
	{
		iret = fifo_dequeue( &u2_send_ring, &temp );
		if ( 0 == iret )
		{
			USART_SendData( USART3, temp );
		}
		else
		{
			USART_ITConfig( USART3, USART_IT_TXE, DISABLE );
		}
	}	
}
