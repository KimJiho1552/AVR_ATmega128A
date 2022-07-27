uint8_t font[] = {0x3f, 0x06, 0x5b, 0x4f,
				  0x66, 0x6d, 0x7c, 0x07,
				  0x7f, 0x6f, 0x77, 0x7c,
				  0x39, 0x5e, 0x79, 0x71,
				  0x08, 0x80};
int t_cnt = 0, m_cnt = 0;

void segment(uint8_t n) {
	uint8_t N1000, N100, N10, N1;
	int buff;
	N1000 = n / 1000;
	buff = n % 1000;
	N100 = buff / 100;
	buff = buff % 100;
	N10 = buff / 10;
	N1 = buff % 10;
	
	for(int i = 0; i < 30; i++) {
		PORTC = 0x0e; PORTA = font[N1000]; _delay_ms(1);
		PORTC = 0x0d;	PORTA = font[N100];	_delay_ms(1);
		PORTC = 0x0b;	PORTA = font[N10];	_delay_ms(1);
		PORTC = 0x07;	PORTA = font[N1];	_delay_ms(1);
	}
}

ISR(TIMER2_COMP_vect) {
	t_cnt++;
	if(t_cnt >= 1000) {
		t_cnt = 0;
		m_cnt++;
	}
}

int main(void) {
	DDRA = 0xff;
	DDRB = 0x80; //PB7 출력(OC2)
	DDRC = 0x0f;
	
	PORTA = 0xff;
	PORTB = 0x00;
	PORTC = 0x0f;
	
	TCCR2 |= (1 << CS22) | (1 << CS20); //0x05 분주비 1024
	TCCR2 |= (1 << WGM21); //CTC Mode
	TCCR2 |= (1 << COM20); //OC2 출력 토글
	TIMSK |= (1 << OCIE2);
	
	OCR2 = 14;
	
	sei(); //Global Interrupt
	
	while(1)
	{
		segment(m_cnt);
		if(m_cnt == 9999) m_cnt = 0;
	}
}