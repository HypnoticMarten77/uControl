#include <avr/io.h>
#include "usart.h"

/* At 2 MHz SYSclk, 12 BSEL, 0 BSCALE corresponds to 9600 bps */
#define BSEL     (12)
#define BSCALE   (0)


char usartd0_in_char(void)
{
  //Write data to initiate a transfer
  USARTD0.DATA = 0x00;
  
  //Wait for transmission to complete
  while(!(USARTD0.STATUS & USART_RXCIF_bm));
  
  //Return the data in char format
  return USARTD0.DATA;
}

void usartd0_in_string(char * buf)
{
  //Not really going to implement this since we are not reading serial data, only using UART for logging/debugging
}

void usartd0_init(void)
{
  /* Configure relevant TxD and RxD pins. */
	PORTD.OUTSET = PIN3_bm;
	PORTD.DIRSET = PIN3_bm;
	PORTD.DIRCLR = PIN2_bm;

  /* Configure baud rate. */
	USARTD0.BAUDCTRLA = (uint8_t)BSEL;
	USARTD0.BAUDCTRLB = (uint8_t)((BSCALE << 4)|(BSEL >> 8));

  /* Configure remainder of serial protocol. */
  /* (In this example, a protocol with 8 data bits, ODD parity, and
   *  one stop bit is chosen.) */
	/*USARTD0.CTRLC =	(USART_CMODE_ASYNCHRONOUS_gc |
					 USART_PMODE_ODD_gc  	 |
					 USART_CHSIZE_8BIT_gc)       &
					 ~USART_SBMODE_bm; */
	
	USARTD0.CTRLC = (USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc) & ~USART_SBMODE_bm;

  /* Enable receiver and/or transmitter systems. */
	USARTD0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;

}

void usartd0_out_char(char c)
{
	while(!(USARTD0.STATUS & USART_DREIF_bm));
	USARTD0.DATA = c;
}

void usartd0_out_int(int8_t c)
{
	//Convert to ASCII
	c += 0x30;
	while(!(USARTD0.STATUS & USART_DREIF_bm));
	USARTD0.DATA = c;
}

void usartd0_out_string(const char * str)
{
	while(*str)
	{
		 usartd0_out_char(*(str++));
	}
}
