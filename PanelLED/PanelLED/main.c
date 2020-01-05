#define F_CPU 8000000UL // Set CPU Clock 8 MHz
#define WIDTH 16 // Matrix Width
#define HEIGHT 8 // Matrix Height

#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include "utility.h"
#include "graphics.h"

// Global Variables Declaration
unsigned char mat_1[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // Memory For First Matrix
unsigned char mat_2[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // Memory For Second Matrix

int main(void)
{
	/* Set Registries as Output */
	DDRA = (1 << DDA7) | (1 << DDA6) | (1 << DDA5) | (1 << DDA4) | (1 << DDA3) | (1 << DDA2) | (1 << DDA1) | (1 << DDA0); // Row 1, 2 Output
	DDRB = (1 << DDA7) | (1 << DDA6) | (1 << DDA5) | (1 << DDA4) | (1 << DDA3) | (1 << DDA2) | (1 << DDA1) | (1 << DDA0); // Column 1 Output
	DDRD = (1 << DDA7) | (1 << DDA6) | (1 << DDA5) | (1 << DDA4) | (1 << DDA3) | (1 << DDA2) | (1 << DDA1) | (1 << DDA0); // Column 2 Output
	
	// Initialize
	rect(2, 2, 6, 4);
	line(9, 1, 14, 6);
	
	// Main Loop
	while (1)
	{
		render();
	}
}

void render()
{
	unsigned char i;
	for (i = 0; i < 8; i++)
	{
		unsigned char temp = ~(1 << i); // Loop in rows ...
		PORTA = temp;
		PORTB = mat_1[i];
		PORTD = mat_2[i];
		_delay_ms(2);
	}
}

void gotoxy(char x, char y, char value)
{
	clamp(&x, 0, 15);
	clamp(&y, 0, 7);
	if (x < 8)
	{
		port(&mat_1[7 - y], value, 7 - x);
	}
	else
	{
		port(&mat_2[7 - y], value, 15 - x);
	}
}

void clear()
{
	unsigned char i;
	for (i = 0; i < 8; i++)
	{
		mat_1[i] = 0x00;
		mat_2[i] = 0x00;
	}
}

void line(char x1, char y1, char x2, char y2)
{
	signed dx = x2 - x1;
	signed char dy = y2 - y1;
	unsigned char steps = maximum(absolute(dx), absolute(dy));
	float x_inc = dx / (float) steps;
	float y_inc = dy / (float) steps;
	unsigned char i;
	float x = x1, y = y1;
	for (i = 0; i <= steps; i++)
	{
		gotoxy(x, y, 1);
		x += x_inc;
		y += y_inc;
	}
}

void rect(char x, char y, char width, char height)
{
	width--;
	height--;
	line(x, y, x + width, y); // Down Line
	line(x, y + height, x + width, y + height); // Up Line
	line(x, y, x, y + height); // Left Line
	line(x + width, y, x + width, y + height); // Right Line
}

void fill_rect(char x, char y, char width, char height)
{
	
}