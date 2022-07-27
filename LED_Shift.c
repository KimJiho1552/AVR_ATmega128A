#define F_CPU 14745600L
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB = 0b11111111; //0xff
	int num = 0;
	while (1)
	{
		PORTB = 1 << num;
		_delay_ms(1000);
		num++;
		if(PORTB == 0x80) {
			PORTB = 0x01;
			num = 1;
			_delay_ms(1000);
		}
	}
}