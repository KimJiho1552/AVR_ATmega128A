#define F_CPU 14745600L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

uint8_t font[] = {0x3f, 0x06, 0x5b, 0x4f,
		0x66, 0x6d, 0x7c, 0x07,
		0x7f, 0x6f, 0x77, 0x7c,
		0x39, 0x5e, 0x79, 0x71,
		0x08, 0x80};
int count = 0;

void segment(uint8_t n) { //FND 출력
	uint8_t N10, N1;
	N10 = n / 10;
	N1 = n % 10;
	
	for(int i = 0; i < 30; i++) {
		PORTG = 0x0b;
		PORTB = font[N10];
		_delay_ms(1);
		
		PORTG = 0x07;
		PORTB = font[N1];
		_delay_ms(1);
	}
}

ISR(INT0_vect) { //Interrupt
	count++;
}

int main(void) {
	DDRB = 0xff; //FND
	DDRG = 0x0f; //FND 제어
	DDRD = 0x00; //0bXXXX XXX0(PD0)
	
	EIMSK = 0x01; //INT0
	//EIMSK |= (1 << INT0);

	EICRA = 0x03; //INT0 RISING EDGE
	//EICRA |= (1 << ISC00) | (1 << ISC01); //RISING
	//EICRA |= (1 << ISC01); //FALLING

	sei(); //Global Interrupt
	//SREG = 0x80;
	
	while(1) {
		segment(count);
		if(count == 99) count = 0;
	}
}