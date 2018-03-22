#include <math.h>

inline long roundup(long num, unsigned long base)
{
	unsigned long r = num % base;
	if (r == 0)
		return num;
	else
	{
		num -= r;
		if (num >= 0)
			num += base;
		return num;
	}
}

inline long rounddown(long num, unsigned long base)
{
	unsigned long r = num % base;
	if (r == 0)
		return num;
	else
	{
		num -= r;
		if (num <= 0)
			num -= base;
		return num;
	}
}