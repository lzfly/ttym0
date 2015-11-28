
#include <stdio.h>
#include <stdint.h>
#include "stm32f0xx_conf.h"

#include "myuart.h"
#include "mymsg.h"

uint32_t  tvvvv = 0;

void  mdelay(__IO uint32_t nTime)
{ 
  __IO uint32_t TimingDelay;
  TimingDelay = nTime * 48000;

  while((TimingDelay--) != 0);
}



/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
static void RCC_Configuration(void)
{
  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC, ENABLE );
  
  /* Enable USARTs Clock */
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE );
	
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE );
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3, ENABLE );
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART4, ENABLE );
	
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_WWDG, ENABLE );
}


/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
static void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
	/* 433 RST : PA4 */
	/* BLE RST : PB2 */	
  /* zigbee RST : PC13 */	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/**/
	GPIO_ResetBits( GPIOA, GPIO_Pin_4 );
	GPIO_ResetBits( GPIOB, GPIO_Pin_2 );
	GPIO_ResetBits( GPIOC, GPIO_Pin_13 );
	mdelay( 100 );
	GPIO_SetBits( GPIOA, GPIO_Pin_4 );
	GPIO_SetBits( GPIOB, GPIO_Pin_2 );
	GPIO_SetBits( GPIOC, GPIO_Pin_13 );
	
	
  /* USART1 Pins configuration ************************************************/
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1); 
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);    
  
  /* Configure pins as AF pushpull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/**/
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_4); 
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_4);    
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1); 
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);  
  
  /* Configure pins as AF pushpull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  /* USART1 Pins configuration ************************************************/
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_4); 
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_4);    
  
  /* Configure pins as AF pushpull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
}




/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
static void NVIC_Configuration(void)
{

#if  0	
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* USART1 IRQ Channel configuration */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif
	

	/**/
	// NVIC_SetPriorityGrouping( 0x3 );

	/**/
	NVIC_SetPriority( SVC_IRQn, (1<<__NVIC_PRIO_BITS) - 2);

	/**/
	NVIC_SetPriority( USART1_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
	NVIC_EnableIRQ( USART1_IRQn );

	NVIC_SetPriority( USART2_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
	NVIC_EnableIRQ( USART2_IRQn );
	
	NVIC_SetPriority( USART3_4_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
	NVIC_EnableIRQ( USART3_4_IRQn );

	/**/
	NVIC_SetPriority( PendSV_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
	
}


int  main( void )
{
	uint8_t  tary[3];
	
	/**/
	RCC_Configuration();
	GPIO_Configuration();
	NVIC_Configuration();
	
	/**/
	my_uart_init();
	
	
	/**/
	tary[0] = 0x55;
	tary[1] = 0xAA;
	msg_send_to_host( 0, 2, tary );
	
	/* 48M / 8 = 6000000 */
	SysTick_Config( 6000000 );
	
	// GPIO_SetBits( GPIOA, GPIO_Pin_1 );

#if 1
	// APB clock (PCLK) = HCLK / RCC_CFGR.PPRE = 48M.
	// 48M / 4096 = 11718,  11718 / 8 = 1464
	// 0x7F - 0x3F = 0x40 = 64, 1464 / 64 = 23 
	// 1 / 23 = 43.7158 ms
	WWDG_SetPrescaler( WWDG_Prescaler_8 );
	WWDG_SetWindowValue( 0x7f );
	WWDG_Enable( 0x7f );
#endif

	/**/
	while(1)
	{
		WWDG_SetCounter( 0x7f );
		__asm( "nop" );
	}
	
	return 0;
}




