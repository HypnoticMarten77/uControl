#include <avr/io.h>
#include <xc.h>

#define RST_bm     (1<<3)


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
		
		spi_write(0x10);
		spi_write(0x00);
		spi_write(0x0A);
		spi_write(0x03);
		spi_write_string("ATI");
		
		usartd0_out_char(SPIF.DATA);
		
// 		spi_write('A');
// 		spi_write('T');
// 		uint8_t temp = spi_read();
// 		usartd0_out_char(temp);
// 		temp = spi_read();
// 		usartd0_out_char(temp);
// 		temp = spi_read();
// 		usartd0_out_char(temp);
// 		temp = spi_read();
// 		usartd0_out_char(temp);
// 		
// 		usartd0_out_string("\n");
		
		/*spi_write(0x41); //A
		spi_write(0x54); //T
		spi_write(0x2B); //+
		spi_write(0x42); //B
		spi_write(0x4C); //L
		spi_write(0x45); //E
		spi_write(0x53); //S
		spi_write(0x54); //T
		spi_write(0x41); //A
		spi_write(0x52); //R
		spi_write(0x54); //T
		spi_write(0x41); //A
		spi_write(0x44); //D
		spi_write(0x56); //V
		spi_write(0x3D); //=
		spi_write(0x3F); //?
		
		//spi_read();
		//spi_read();
		//spi_read();
		//spi_read();
		//spi_read();
		//spi_read();
		
        //TODO:: Please write your application code 
		
		*/
    }
}