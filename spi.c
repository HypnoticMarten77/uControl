/*------------------------------------------------------------------------------
  spi.c --
  
  Description:
    Provides useful definitions for manipulating the relevant SPI
    module of the ATxmega128A1U. 

------------------------------------------------------------------------------*/

/********************************DEPENDENCIES**********************************/

#include <avr/io.h>
#include "spi.h"

/*****************************END OF DEPENDENCIES******************************/


/*****************************FUNCTION DEFINITIONS*****************************/


void spi_init(void)
{
	
  /* Initialize the relevant SPI output signals to be in an "idle" state.
   * Refer to the relevant timing diagram within the LSM6DSL data sheet.
   * (You may wish to utilize the macros defined in `spi.h`.) */
  //Slave select is active low so idle would be set high
  PORTF.OUTSET = (SS_bm);

  /* Configure the pin direction of relevant SPI signals. */
  //DIRSET = OUTPUTS
  PORTF.DIRSET = (SS_bm | MOSI_bm | SCK_bm);
 	PORTF.DIRCLR = (MISO_bm);
	
	/* Set the other relevant SPI configurations. */
	SPIF.CTRL	=	(SPI_PRESCALER_DIV16_gc	|  SPI_MASTER_bm |  SPI_MODE_3_gc  |  SPI_ENABLE_bm);
}

void spi_write(uint8_t data)
{
	//Write data
	SPIF.DATA = data;
	
	//Wait for transmission to complete
	while(!(SPIF.STATUS & SPI_IF_bm));

}

uint8_t spi_read(void)
{
	/* Write some arbitrary data to initiate a transfer. */
	SPIF.DATA = 0x37;

	//Wait for transmission to complete
	while(!(SPIF.STATUS & SPI_IF_bm));
	
	/* After the transmission, return the data that was received. */
	return SPIF.DATA;
}

/***************************END OF FUNCTION DEFINITIONS************************/
