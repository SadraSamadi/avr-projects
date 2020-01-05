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
	spi_mi();
	write_reg(&DDRC, 0, 0);
	write_reg(&PORTC, 1, 0);
	while (1)
	{
		char input = read_reg(PINC, 0);
		if (input == 0)
		{
			spi_msd(0xFF);
		}
		else
		{
			spi_msd(0x00);
		}
	}
}