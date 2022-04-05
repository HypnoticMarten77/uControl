/*------------------------------------------------------------------------------
  spi.c --
  
  Description:
    Provides useful definitions for manipulating the relevant SPI
    module of the ATxmega128A1U. 
  
  Author(s): Dr. Eric M. Schwartz, Christopher Crary, Wesley Piard
  Last modified by: Mason Anderson
  Last modified on: 21 March 2021
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
	PORTF.OUTCLR = SS_bm;
	
	for(int i = 0; i < 1000; i++); //Add delay
	
	//Write data
	SPIF.DATA = data;
	
	//Wait for transmission to complete
	while(!(SPIF.STATUS & SPI_IF_bm));
	PORTF.OUTSET = SS_bm;
	
	

  /* In general, it is probably wise to ensure that the relevant flag is
   * cleared at this point, but, for our contexts, this will occur the 
   * next time we call the `spi_write` (or `spi_read`) routine. 
   * Really, because of how the flag must be cleared within
   * ATxmega128A1U, it would probably make more sense to have some single 
   * function, say `spi_transceive`, that both writes and reads 
   * data, rather than have two functions `spi_write` and `spi_read`,
   * but we will not concern ourselves with this possibility
   * during this semester of the course. */

}

uint8_t spi_read(void)
{
	
	PORTF.OUTCLR = SS_bm;
	for(int i = 0; i < 1000; i++); //Add delay
	
	/* Write some arbitrary data to initiate a transfer. */
	SPIF.DATA = 0x00;

	//Wait for transmission to complete
	while(!(SPIF.STATUS & SPI_IF_bm));
	
	PORTF.OUTSET = SS_bm;
	
	/* After the transmission, return the data that was received. */
	return SPIF.DATA;
}

/***************************END OF FUNCTION DEFINITIONS************************/
