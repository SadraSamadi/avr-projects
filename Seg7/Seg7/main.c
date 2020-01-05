#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

#define F_CPU 8000000UL  // Set CPU Clock 8MHz
#include <util/delay.h>

/* Global variables definition */
unsigned char _index;
const char dec_to_7seg[10] = {
	0xC0,	// 0
	0xF9,	// 1
	0xA4,	// 2
	0xB0,	// 3
	0x99,	// 4
	0x92,	// 5
	0x82,	// 6
	0xD8,	// 7
	0x80,	// 8
	0x90	// 9
};
char _7seg_temp[4] = {
	0xFF,	// Part 1
	0xFF,	// Part 2
	0xFF,	// Part 3
	0xFF	// Part 4
};

/* Functions definition */
void print_number(short n);
void remove_left_zero(void);

/* Timer0 overflow interrupt (every 1ms will be called) */
ISR(TIMER0_OVF_vect)
{
	/* Stop timer0 */
	TCCR0 = (0 << FOC0) | (0 << WGM00) | (0 << COM01) | (0 << COM00) | (0 << WGM01) | (0 << CS02) | (0 << CS01) | (0 << CS00);
	/* Initialize timer0 */
	TCNT0 = 0xC1; // 193
	/************************************************************************/
	PORTC = (1 << _index);
	PORTB = _7seg_temp[_index];
	_index = _index < 4 ? _index + 1 : 0;
	/************************************************************************/
	/* Start timer0 with 1/64 CPU clock */
	TCCR0 = (0 << FOC0) | (0 << WGM00) | (0 << COM01) | (0 << COM00) | (0 << WGM01) | (0 << CS02) | (1 << CS01) | (1 << CS00);
}

int main()
{
	/* Local variables definition */
	unsigned int counter = 0;
	
	/* Set register 'A' as input */
	DDRA = (0 << DDA7) | (0 << DDA6) | (0 << DDA5) | (0 << DDA4) | (0 << DDA3) | (0 << DDA2) | (0 << DDA1) | (0 << DDA0);
	/* Set register 'B' as output */
	DDRB = (1 << DDB7) | (1 << DDB6) | (1 << DDB5) | (1 << DDB4) | (1 << DDB3) | (1 << DDB2) | (1 << DDB1) | (1 << DDB0);
	/* Set register 'C' as output */
	DDRC = (1 << DDC7) | (1 << DDC6) | (1 << DDC5) | (1 << DDC4) | (1 << DDC3) | (1 << DDC2) | (1 << DDC1) | (1 << DDC0);
	
	/* Enable pull up resistor for pin 'A' */
	PORTA = (1 << PORTA7) | (1 << PORTA6) | (1 << PORTA5) | (1 << PORTA4) | (1 << PORTA3) | (1 << PORTA2) | (1 << PORTA1) | (1 << PORTA0);
	
	/* Initialize timer0 */
	TCNT0 = 0xC1; // 193
	/* Enable timer0 interrupt */
	TIMSK = (0 << OCIE2) | (0 << TOIE2) | (0 << TICIE1) | (0 << OCIE1A) | (0 << OCIE1B) | (0 << TOIE1) | (0 << OCIE0) | (1 << TOIE0);
	/* Start timer0 with 1/64 CPU clock */
	TCCR0 = (0 << FOC0) | (0 << WGM00) | (0 << COM01) | (0 << COM00) | (0 << WGM01) | (0 << CS02) | (1 << CS01) | (1 << CS00);
	
	/* Initialize */
	print_number(0);
	sei();
	
	/* Program main loop */
	while (1)
	{
		/* Listen to PINA0 */
		if (PINA == 0xFE)
		{
			print_number(++counter);
			_delay_ms(100);
		}
	}
}

/* Functions implementation */

void print_number(short n)
{
	unsigned char i;
	for (i = 0; i < 4; i++)
		_7seg_temp[i] = dec_to_7seg[(n / (short) pow(10, i)) % 10];
	remove_left_zero();
}

void remove_left_zero()
{
	unsigned char i;
	for (i = 3; i > 0; i--)
		if (_7seg_temp[i] == dec_to_7seg[0])
			_7seg_temp[i] = 0xFF;
		else
			break;
}
