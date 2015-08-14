#include <stdio.h>
#include <stdint.h>
#include "stm32f0xx_conf.h"

#include "mymsg.h"
#include "systick.h"



/* 1:rf433, 2-ble, 3-zigbee */
static  int  count[4] = { 0,0,0,0 };

/**/
void  mytick_reset_mod( int idx )
{
	/**/
	if ( (idx < 0) || (idx > 3) )
	{
		return;
	}
	
	/**/
	switch( idx )
	{
		case 0:
			while(1);
			break;
		
		case 1:
			count[idx] = 4;
			GPIO_ResetBits( GPIOA, GPIO_Pin_4 );
			break;
		
		case 2:
			count[idx] = 4;
			GPIO_ResetBits( GPIOB, GPIO_Pin_2 );
			break;
		
		case 3:
			count[idx] = 4;
			GPIO_ResetBits( GPIOC, GPIO_Pin_13 );
			break;
	}
	
	return;
}


void  mytick_check_reset( void )
{
	if ( count[1] > 0 )
	{
		count[1] -= 1;
		if ( count[1] == 0 )
		{
				GPIO_SetBits( GPIOA, GPIO_Pin_4 );
		}
	}
	
	if ( count[2] > 0 )
	{
		count[2] -= 1;
		if ( count[2] == 0 )
		{
				GPIO_SetBits( GPIOB, GPIO_Pin_2 );
		}
	}	
	
	if ( count[3] > 0 )
	{
		count[3] -= 1;
		if ( count[3] == 0 )
		{
				GPIO_SetBits( GPIOC, GPIO_Pin_13 );
		}
	}
	
}


void  SysTick_Handler( void )
{
	
#if 0
	uint8_t  tary[4];
	
	tary[0] = 0x31;
	tary[1] = 0x32;
	tary[2] = 0x0D;
	tary[3] = 0x0A;
	
	msg_send_to_host( 0, 4, tary );
#else
	mytick_check_reset();
#endif
	
	return;
}



