#include <avr/io.h>

#define F_CPU 1000000UL
#include <util/delay.h>

#define DDR_SPI DDRB
#define	DD_MOSI DDB3
#define DD_MISO DDB4
#define DD_SCK DDB5
#include "spi.h"

char read_reg(volatile uint8_t reg, char pos)
{
	return reg & (1 << pos) ? 1 : 0;
}

void write_reg(volatile uint8_t *reg, char value, char pos)
{
	if (value)
	{
		*reg |= (1 << pos);
	}
	else
	{
		*reg &= ~(1 << pos);
	}
}

int main()
{
	spi_si();
	write_reg(&DDRC, 1, 0);
	write_reg(&PORTC, 0, 0);
	while (1)
	{
		char input = spi_srd();
		if (input == 0xFF)
		{
			write_reg(&PORTC, 1, 0);
		} 
		else if (input == 0x00)
		{
			write_reg(&PORTC, 0, 0);
		}
	}
}