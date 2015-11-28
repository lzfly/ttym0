
#include <stdint.h>
#include <stdio.h>

#include "stm32f0xx_usart.h"

#include "myuart.h"
#include "mymsg.h"
#include "mysvch.h"


__asm  int  testsvc( intptr_t func, intptr_t arg0, intptr_t arg1 )
{
	PUSH	{ r0 }
	MOV 	r3,	r2
	MOV		r2, r1
	MOV		r1,	sp
	SVC		#0
	POP		{ r0 }
	BX		lr
}



void  SVC_Handler( intptr_t func, intptr_t pret, intptr_t arg0, intptr_t arg1 )
{
	int  iret;

	/**/
  switch ( func )
	{
	case 0:
		/* trig send to uart0, host */
		if( (USART1->CR1 & USART_CR1_TXEIE) == 0 )
		{
			USART_ITConfig( USART1, USART_IT_TXE, ENABLE );
			USART_ClearFlag( USART1, USART_FLAG_TC );
		}
		iret = 0;
		break;
  
	case 1:
		if( (USART2->CR1 & USART_CR1_TXEIE) == 0 )
		{
			USART_ITConfig( USART2, USART_IT_TXE, ENABLE );
			USART_ClearFlag( USART2, USART_FLAG_TC );
		}		
		iret = 0;
		break;
	
	case 2:
		if( (USART3->CR1 & USART_CR1_TXEIE) == 0 )
		{
			USART_ITConfig( USART3, USART_IT_TXE, ENABLE );
			USART_ClearFlag( USART3, USART_FLAG_TC );
		}	
		iret = 0;
		break;

  case 3:
		if( (USART4->CR1 & USART_CR1_TXEIE) == 0 )
		{
			USART_ITConfig( USART4, USART_IT_TXE, ENABLE );
			USART_ClearFlag( USART4, USART_FLAG_TC );
		}	
		iret = 0;
		break;
	
	case 4:
		iret = 0;
		break;
	
	case 5:
		my_uart_reinit( USART3, (uint32_t)arg0 );
		iret = 0;
		break;
	
	case 6:
		my_uart_reinit( USART4, (uint32_t)arg0 );
		iret = 0;
		break;
	
	case 7:
		my_uart_reinit( USART4, (uint32_t)arg0 );
		iret = 0;
		break;
	
	default:
		iret = -1;
		break;
	}
	
	*(int *)pret = iret;
	return;
}


