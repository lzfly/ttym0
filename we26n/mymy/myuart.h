
#ifndef  __MY_UART_H_
#define  __MY_UART_H_


int  my_uart_init( void );

/**/
int  my_uart_send( int tidx, int tlen, uint8_t * pdat );

#endif

