#include <avr/io.h>
#include <xc.h>

#define RST_bm     (1<<3)
#define SS_bm     (1<<4)


void tcc0_init(void)
{
//Load digital period (0.25secs)*(2MHz/8) = 62500; 4Hz
TCC0.PER = 7812;

//CH0 Event trigger on overflow
//EVSYS_CH0MUX = EVSYS_CHMUX_TCC0_OVF_gc;

//Load prescaler to start the timer prescaler of 8
TCC0.CTRLA = TC_CLKSEL_DIV256_gc;
}


void reset_module(void){
	PORTF.OUTCLR = (RST_bm);
	
	TCC0.INTFLAGS = (1); //Reset the interrupt flag and then begin polling;
	
	while(!(TCC0.INTFLAGS & 1));
	
	PORTF.OUTSET = (RST_bm);
}


int main(void)
{
	
	tcc0_init();
	
	PORTF.DIRSET = (RST_bm);
	PORTF.OUTSET = (RST_bm);
	
	reset_module();
	
	
	
	spi_init();
	usartd0_init();
	
		//const char * command  = "AT";
		
		//spi_write_string(command);
		
		//reset_module();
		
		//usartd0_out_string("Sent command: ");
		//usartd0_out_string(command);
		//usartd0_out_string("\r\n");
		
		
		
		//usartd0_out_char(spi_read());
	
	
	
    while(1)
    {
		
		//while(!(TCC0.INTFLAGS & 1)); //Poll timer to check for overflow
		//TCC0.INTFLAGS = 1; //Reset the timer
		//usartd0_out_char(spi_read());
		
		//spi_write_string("ATI");
		//usartd0_out_char(spi_read());
		PORTF.OUTCLR = SS_bm;
		
		spi_write(0x10);           //10-01-0A-nBytes-0byte-1byte-...-nbyte this sends to uart
		spi_write(0x01);
		spi_write(0x0A);
		spi_write(0x03);
		spi_write_string("Poo");
		
		PORTF.OUTSET = SS_bm;
		
		//PORTF.OUTCLR = SS_bm;
// 		usartd0_out_char(spi_read());
// 		usartd0_out_char(spi_read());
// 		usartd0_out_char(spi_read());
// 		usartd0_out_char(spi_read());
// 		usartd0_out_char(spi_read());
// 		usartd0_out_char(spi_read());
// 		usartd0_out_char(spi_read());
// 		PORTF.OUTSET = SS_bm;
		
		//usartd0_out_char(SPIF.DATA);
		

    }
}