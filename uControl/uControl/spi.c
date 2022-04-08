#include <avr/io.h>
#include "spi.h"

void spi_init(void)
{
	
	//Slave select is active low so idle would be set high
	PORTF.OUTSET = (SS_bm);

	/* Configure the pin direction of relevant SPI signals. */
	//DIRSET = OUTPUTS
	PORTF.DIRSET = (SS_bm | MOSI_bm | SCK_bm);
 	PORTF.DIRCLR = (MISO_bm);
	
	/* Set the other relevant SPI configurations. */
	SPIF.CTRL	=	(SPI_PRESCALER_DIV16_gc	|  SPI_MASTER_bm |  SPI_MODE_0_gc  |  SPI_ENABLE_bm);
}


void spi_write_string(const char * str){
	while(*str)
	{
		spi_write(*(str++));
	}
}


void spi_write(uint8_t data)
{

	
	//Write data to the buffer to initiate a transfer.
	SPIF.DATA = data;
	
	//Wait for transmission to complete
	while(!(SPIF.STATUS & SPI_IF_bm));
	//PORTF.OUTSET = SS_bm;
	
}

uint8_t spi_read(void)
{
	
	
	/* Write some arbitrary data to initiate a transfer. */
	SPIF.DATA = 0x01;

	//Wait for transmission to complete by polling the interrupt flag
	while(!(SPIF.STATUS & SPI_IF_bm));
	
	//PORTF.OUTSET = SS_bm;
	
	/* After the transmission, return the data that was received. */
	return SPIF.DATA;
}
