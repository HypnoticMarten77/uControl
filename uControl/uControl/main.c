#include <avr/io.h>
#include <avr/interrupt.h>
#include <xc.h>


#define SS_bm     (1<<4)
#define IRQ_bm	(1<<3)



void int_init(void)
{
	//Enable low level interrupts
	PMIC.CTRL = PMIC_LOLVLEN_bm;
	//Enable global interrupts
	sei();
	PORTF.DIRCLR = IRQ_bm; //Make input
	PORTF.OUTCLR = IRQ_bm;
	PORTF.INTCTRL = 0x01; //Low level interrupts on PORTF
	PORTF.INT0MASK = IRQ_bm; //PIN3 Interrupt enable
	PORTF.PIN3CTRL = PORT_ISC_RISING_gc; //Sense only on rising edge
	
	//PORTF.INTFLAGS INT0IF is flag
	
	
}

ISR(PORTF_INT0_vect){
	
	//Every time a packet is ready in the buffer of the device, we need to read it in.
	
 	//PORTF.OUTTGL = 1;
	PORTF.OUTCLR = SS_bm;
	usartd0_out_char(spi_read());
	usartd0_out_char(spi_read());
	usartd0_out_char(spi_read());
	usartd0_out_char(spi_read());
	usartd0_out_char(spi_read());
	usartd0_out_char(spi_read());
	usartd0_out_char(spi_read());
	usartd0_out_char(spi_read());
	usartd0_out_char(spi_read());
	usartd0_out_char(spi_read());
	usartd0_out_char(spi_read());
	usartd0_out_char(spi_read());
	PORTF.OUTSET = SS_bm;
	
	PORTF.INTFLAGS = 1; //Reset interrupt flag
}



void tcc0_init(void)
{
//Load digital period (0.25secs)*(2MHz/8) = 62500; 4Hz
TCC0.PER = 7812;

//CH0 Event trigger on overflow
//EVSYS_CH0MUX = EVSYS_CHMUX_TCC0_OVF_gc;

//Load prescaler to start the timer prescaler of 8
TCC0.CTRLA = TC_CLKSEL_DIV256_gc;
}


int main(void)
{
	int_init();
	tcc0_init(); //Initialize Timer Counter Module 0
	spi_init();
	usartd0_init();
	
	PORTF.DIRSET = 1;
	
    while(1)
    {
		

		while(!(TCC0.INTFLAGS & 1)); //Poll timer to check for overflow
		TCC0.INTFLAGS = 1; //Reset the timer
		
		//PORTF.OUTCLR = SS_bm;
		//usartd0_out_char(spi_read());
		//PORTF.OUTSET = SS_bm;
		
		//spi_write_string("ATI");
		//usartd0_out_char(spi_read());
		
		
		PORTF.OUTCLR = SS_bm;
	
		spi_write(0x10);           //10-01-0A-nBytes-0byte-1byte-...-nbyte this sends to uart
		spi_write(0x02);
		spi_write(0x0A);
		spi_write(0x00);
		//spi_write_string("This is a test.\n");
		
		PORTF.OUTSET = SS_bm;
	
	
		
		
		
		


    }
}