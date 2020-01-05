#ifndef SPI_H_
#define SPI_H_

#ifndef _AVR_IO_H_
#include <avr/io.h>
#endif

#ifndef DDR_SPI
#warning "DDR_SPI not defined !"
#define DDR_SPI DDRB // Config SPI register default for ATmega16
#endif // DDR_SPI

#ifndef DD_MOSI
#warning "DD_MOSI not defined !"
#define DD_MOSI DDB5 // Config MOSI pin default for ATmega16
#endif // DD_MOSI

#ifndef DD_MISO
#warning "DD_MISO not defined !"
#define DD_MISO DDB6 // Config MISO pin default for ATmega16
#endif // DD_MOSI

#ifndef DD_SCK
#warning "DD_SCK not defined !"
#define DD_SCK DDB7 // Default Config SCK pin default for ATmega16
#endif // DD_MOSI

/* SPI master initialize */
void spi_mi()
{
	/* Set MOSI and SCK output, all others input */
	DDR_SPI = (1 << DD_MOSI) | (1 << DD_SCK);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

/* SPI master send data */
void spi_msd(char data)
{
	/* Start transmission */
	SPDR = data;
	/* Wait for transmission complete */
	while(!(SPSR & (1 << SPIF)));
}

/* SPI slave initialize */
void spi_si()
{
	/* Set MISO output, all others input */
	DDR_SPI = (1 << DD_MISO);
	/* Enable SPI */
	SPCR = (1 << SPE);
}

/* SPI slave receive data */
char spi_srd()
{
	/* Wait for reception complete */
	while(!(SPSR & (1 << SPIF)));
	/* Return data register */
	return SPDR;
}

#endif /* SPI_H_ */