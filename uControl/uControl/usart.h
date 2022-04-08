#ifndef USART_H
#define USART_H

#include <avr/io.h>


char usartd0_in_char(void);

void usartd0_in_string(char * buf);


void usartd0_init(void);


void usartd0_out_char(char c);


void usartd0_out_int(int8_t c);


void usartd0_out_string(const char * str);

#endif