#include "utility.h"

char minimum(char a, char b)
{
	return a < b ? a : b;
}

char maximum(char a, char b)
{
	return a > b ? a : b;
}

void clamp(char *value, char min, char max)
{
	if (*value < min)
	{
		*value = min;
	}
	else if (*value > max)
	{
		*value = max;
	}
}

unsigned char absolute(signed char a)
{
	return a > 0 ? a : -a;
}

void port(unsigned char *reg, char value, char position) {
	if (value)
	{
		*reg |= (1 << position);
	}
	else
	{
		*reg &= ~(1 << position);
	}
}