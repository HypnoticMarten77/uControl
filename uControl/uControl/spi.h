#ifndef SPI_H_
#define SPI_H_


#include <avr/io.h>

#define SS_bm     (1<<4)
#define MOSI_bm	  (1<<5)
#define MISO_bm	  (1<<6)
#define SCK_bm    (1<<7)


void spi_init(void);

void spi_write(uint8_t data);

uint8_t spi_read(void);

#endif