#include <avr/io.h>
#define F_CPU 1000000
#include <util/delay.h>

int main(void)
{
	DDRA = 0xFF;
	DDRB = 0xFF;
	unsigned char array[10] =
	{0b00111111,   // 0
	 0b00000110,   // 1
	 0b01011011,   // 2
	 0b01001111,   // 3
	 0b01100110,   // 4
	 0b01101101,   // 5
	 0b01111101,   // 6
	 0b00000111,   // 7
	 0b01111111,   // 8
	 0b01101111};  // 9
	unsigned char t1 = 0, t2 = 0;
    while (1) {
		PORTA = array[t1];
		PORTB = array[t2];
		t1++;
		_delay_ms(1000);
		if (t1 > 9) {
			t2++;
			t1 = 0;
		}
		if (t2 > 9) {
			t1 = 0;
			t2 = 0;
		}
    }
}