#include <avr/io.h>
#include <avr/interrupt.h>
#include <xc.h>

//DEFINE BITMASKS
#define SS_bm   (1<<4)
#define IRQ_bm	(1<<3)


void int_init(void)
{
	//Enable low level interrupts
	PMIC.CTRL = PMIC_LOLVLEN_bm;
	
	//Enable global interrupts
	sei();
	
	//Set the interrupt pin as an input and de-assert it for initialization.
	PORTF.DIRCLR = IRQ_bm;
	PORTF.OUTCLR = IRQ_bm;
	
	//Low level interrupts on PORTF
	PORTF.INTCTRL = 0x01;
	
	//PIN3 Interrupt enable
	PORTF.INT0MASK = IRQ_bm; 
	
	//Sense interrupts only on rising edge
	PORTF.PIN3CTRL = PORT_ISC_RISING_gc;
	
	//PORTF.INTFLAGS INT0IF is interrupt0 flag for PORTF
	
}

//Interrupt Service Routine for PORTF Interrupt0 on PIN3
ISR(PORTF_INT0_vect){
	
	//Every time a packet is ready in the buffer of the device, we need to read it in.
	PORTF.OUTCLR = SS_bm;
	spi_read();
	spi_read();
	spi_read();
	


	uint8_t num_bytes = spi_read();
	
	//Only display if we have actually received new data.
	//Careful, do not flood the transmission with more than 13 bytes, otherwise it breaks.
	
	if(num_bytes){
	
	//usartd0_out_string("Bytes received: ");
	//usartd0_out_int(num_bytes - 1);
	//usartd0_out_char(' ');
	
	
	usartd0_out_string("Received: ");
	
	uint8_t char1 = spi_read();
	uint8_t char2 = spi_read();
	uint8_t char3 = spi_read();
	
	usartd0_out_char(char3);
	
	
	if(char3 == 'U') PORTA.OUTTGL = 0x01;
	if(char3 == 'D') PORTA.OUTTGL = 0x02;
	if(char3 == 'L') PORTA.OUTTGL = 0x04;
	if(char3 == 'R') PORTA.OUTTGL = 0x08;
	if(char3 == 'N') PORTA.OUTTGL = 0x10;
	
	
	
	//for(int i = 0; i < num_bytes-4; i++){
	//	usartd0_out_char(spi_read());
	//}
	
	
	
	usartd0_out_char('\n');
	
	}
	PORTF.OUTSET = SS_bm;
	
	PORTF.INTFLAGS = 1; //Reset interrupt flag
}



void tcc0_init(void)
{
//Load digital period (1sec1)*(2MHz/256) = 7812; 1Hz period calculation
TCC0.PER = 7812;

//Load prescaler to start the timer prescaler of 8
TCC0.CTRLA = TC_CLKSEL_DIV1_gc;
}


int main(void)
{
	//Initialize Interrupts
	int_init();
	
	//Initialize Timer/Counter Module 0
	tcc0_init();
	
	//Initialize SPI Module
	spi_init();
	
	//Initialize UART Module 0
	usartd0_init();
	
	
	
	PORTA.DIRSET = 31;
	//PORTA.OUTSET = 15;
	
	
	
	
	
    while(1)
    {
		
		//1 SECOND DELAY SO THE UART DEBUG CONSOLE IS NOT FLOODED
		while(!(TCC0.INTFLAGS & 1)); //Poll timer to check for overflow
		TCC0.INTFLAGS = 1; //Clear the Flag
		
		//Read the buffer to tell the module to send an interrupt when there is new data in the buffer.
		PORTF.OUTCLR = SS_bm;
		spi_write(0x10);
		spi_write(0x02);
		spi_write(0x0A);
		spi_write(0x00);
		PORTF.OUTSET = SS_bm;
	

		/*--------------TRANSMISSION EXAMPLE--------------*/
	
// 		PORTF.OUTCLR = SS_bm;
// 		spi_write(0x10);           //10-01-0A-nBytes-0byte-1byte-...-nbyte this sends to uart
// 		spi_write(0x01);
// 		spi_write(0x0A);
// 		spi_write(0x10);
// 		spi_write_string("This is a test.\n");
// 		PORTF.OUTSET = SS_bm;
		
		
		


    }
}