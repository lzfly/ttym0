
#ifndef  __MY_UART_H_
#define  __MY_UART_H_


int  my_uart_init( void );
int  my_uart_reinit( USART_TypeDef * USARTx, uint32_t baud );

/**/
int  my_uart_send( int tidx, int tlen, uint8_t * pdat );
int  my_uart_baudrate( int tidx, uint32_t baud );

#endif

