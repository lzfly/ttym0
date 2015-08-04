

#define _PRINTF_C_SRC_

/**********************************************************************************
**  				            		MODULES USED		  		     		***													  	
**********************************************************************************/ 
#include <stdint.h>
#include <stdarg.h>

#include "myuart.h"
#include "mprintf.h"




/**
 * @fn static void itoa(uint32_t val , uint8_t *buf, uint8_t radix)
 *	integer to ascii
 */
static void itoa(uint32_t val , uint8_t *buf, uint8_t radix)
{
	uint8_t *p;							// pointer to traverse string         
	uint8_t *firstdig;					// pointer to first digit         
	uint8_t temp;						// temp char         
	uint16_t  digval;					// value of digit        
     
	p = buf;       
	firstdig = p;      				// save pointer to first digit 
     
	do 
	{            
		digval = (val % radix);				
    	val /= radix;	            

		// convert to ascii and store
    	if (digval > 9)                
        	*p++ = (char) (digval - 10 + 'A');  	// a letter          
     	else                
			*p++ = (char) (digval + '0');      		// a digit        
	} 
	while (val > 0);        

	// We now have the digit of the number in the buffer, but in reverse order.  Thus we reverse them now.         

	*p-- = '\0';            	// terminate string; p points to last digit        
	do
	{            
        	temp = *p;            
        	*p = *firstdig;            
        	*firstdig = temp;   // swap *p and *firstdig          
        	--p;           
        	++firstdig;         	// advance to next two digits       
       }
	while (firstdig < p); 	// repeat until halfway
	
}



/**
 * @fn static void SerialWriteBuffer(char *str)
 *	
 */
static void SerialWriteBuffer(char *str)
{
	while(*str)
        	my_uart_writec(*(str++));
}


/**********************************************************************************
**					             	EXPORTED FUNCTIONS  		  	          	***													  	
**********************************************************************************/

/**
 * @fn void mprintf(char *fmt, ...)
 * 
 */
void  mprintf( char *fmt, ... )
{
  uint8_t *s;
  uint32_t d;
  uint8_t buf[16];

  va_list ap;                 // pointer that point to parameter list
  va_start(ap, fmt);        // initialize ap

  while(*fmt)
  {
    if(*fmt != '%')
    {
      if((*fmt == '\r') || (*fmt == '\n'))
        SerialWriteBuffer("\r\n");
      else
        my_uart_writec(*fmt); 
    }
    else
    {
      switch(*++fmt)
      {
				case 'c':
					s = va_arg(ap, uint8_t *); 
					my_uart_writec(*s);
					break;

				case 's':
					s = va_arg(ap, uint8_t *);
					for(; *s; s++)
						my_uart_writec(*s);
					break;

				case 'd':
					d = va_arg(ap, int);
					itoa(d, buf, 10);       //Converts an int to a character string.
					for(s = buf; *s; s++)
						my_uart_writec(*s);
          break;
          
				case 'x':
					d = va_arg(ap, int);
					itoa(d, buf, 16);       //Converts an int to a character string.
					for(s = buf; *s; s++)
						my_uart_writec(*s);  
          break;
				
				default:
					my_uart_writec(*fmt);
			}
		}
		fmt++;
  }
  va_end(ap);
	
}






