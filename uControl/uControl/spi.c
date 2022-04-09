#include <avr/io.h>
#include "spi.h"

void spi_init(void)
{
	
	//Slave select is active low so idle would be set high
	PORTF.OUTSET = (SS_bm);

	//PIN Directions for relevant SPI configuration.
	PORTF.DIRSET = (SS_bm | MOSI_bm | SCK_bm);
 	PORTF.DIRCLR = (MISO_bm);
	
	//(16 Prescaler | Master Mode | Setup Rising Sample Falling | Enable SPI)
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

	//Write the payload to the buffer to start a transfer.
	SPIF.DATA = data;
	
	//Wait for transmission to complete
	while(!(SPIF.STATUS & SPI_IF_bm));
	
}

uint8_t spi_read(void)
{
	
	//Write any data to the SPI buffer to initiate a transfer.
	SPIF.DATA = 0x01;

	//Wait for transmission to complete by polling the interrupt flag
	while(!(SPIF.STATUS & SPI_IF_bm));
	
	//Return the data that was received.
	return SPIF.DATA;
}
