#define F_CPU 14745600L
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRE = 0x00; //Input
	DDRD = 0xff; //Output
    while (1)
    {
		PORTD = PINE; //입력 들어온 것만
    }
}