#define F_CPU 14745600L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t font[] = {0x3f, 0x06, 0x5b, 0x4f,
									0x66, 0x6d, 0x7c, 0x07,
									0x7f, 0x6f, 0x77, 0x7c,
									0x39, 0x5e, 0x79, 0x71,
									0x08, 0x80};
void r_segment(int);
void l_segment(int);

int r_tcnt = 0, r_mcnt = 0;
int l_tcnt = 0, l_mcnt = 0;

void r_segment(int n) {
	uint8_t N10, N1;
	N10 = n / 10;
	N1 = n % 10;
	
	for(int i = 0; i < 30; i++) {
		PORTC = 0x0b;	PORTA = font[N10];	_delay_ms(1);
		PORTC = 0x07;	PORTA = font[N1];	_delay_ms(1);
	}
}

void l_segment(int n) {
	uint8_t N10, N1;
	N10 = n / 10;
	N1 = n % 10;
	
	for(int i = 0; i < 30; i++) {
		PORTC = 0x0e;	PORTA = font[N10];	_delay_ms(1);
		PORTC = 0x0d;	PORTA = font[N1];	_delay_ms(1);
	}
}

ISR(TIMER0_COMP_vect) {
	l_tcnt++;
	if(l_tcnt >= 28) { //500ms
		l_tcnt = 0;
		l_mcnt++;
		if(l_mcnt > 99) l_mcnt = 0;
	}
}

ISR(TIMER0_OVF_vect) {
	r_tcnt++;
	if(r_tcnt >= 56) { //1s
		r_tcnt = 0;
		r_mcnt++;
		if(r_mcnt > 99) r_mcnt = 0;
	}
}

int main(void) {
	DDRA = 0xff; //FND
	DDRC = 0x0f; //FND 제어
	PORTA = 0xff;
	PORTC = 0x0f;
	
	DDRB = 0x10; //PB4 출력(OC0)
	TIMSK |= (1 << OCIE0) | (1 << TOIE0); //Interrupt
	TCCR0 |= (1 << WGM00) | (1 << WGM01); //Fast PWM Mode
	TCCR0 |= (1 << COM01); //Clear OC0 on compare match
	TCCR0 |= (1 << CS02) | (1 << CS01) | (1 << CS00); //분주비 1024
	TCNT0 = 0;
	OCR0 = 102; //40% High, 60% Low
	sei(); //Global Interrupt
	
	while(1) {
		r_segment(r_mcnt);
		l_segment(l_mcnt);
	}
}