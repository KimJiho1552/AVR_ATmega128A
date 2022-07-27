#define F_CPU 14745600L
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRE = 0x0f; //0b 0000(입력) 1111(출력)
	while(1)
	{
		PORTE = PINE >> 4;
	}
}