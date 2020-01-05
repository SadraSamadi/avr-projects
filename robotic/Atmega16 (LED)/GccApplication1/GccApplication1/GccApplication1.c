#include <avr/io.h>
#define F_CPU 1000000
#include <util/delay.h>

int main(void)
{
	DDRA = 0xFF;
	DDRC = 0xFF;
    while (1) {
		PORTA = 0b00000001;
		PORTC = 0b00000100;
		_delay_ms(60000);
		
		PORTA = 0b00000010;
		PORTC = 0b00000010;
		_delay_ms(5000);
		
		PORTA = 0b00000100;
		PORTC = 0b00000001;
		_delay_ms(60000);
    }
}