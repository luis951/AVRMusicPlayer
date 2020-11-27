#include<avr/io.h>
#include<avr/interrupt.h>

#include "musicPlayer.h"

volatile song song1;
volatile note *treble;
volatile note *mid;
volatile note *bass;

volatile uint16_t flagBuzzer1=0;
volatile uint16_t flagBuzzer2=0;
volatile uint16_t flagBuzzer3=0;

void initTimer1(){
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	TIMSK1 = 0;

	if(treble != 0){
	    TCCR1A |= (1 << COM1A0); //init do modo toggle da porta OC1A (PB1)
		TCCR1B |= (1 << WGM12)|(1 << CS10); //prescaler = 1
		OCR1A = treble->ocrValue;
		if(treble->frequency==0) DDRB &= ~(1 << PB1);
		TIMSK1 |= (1 << OCIE1A);
	}
}

void initTimer2(){
	TCCR2A = 0;
	TCCR2B = 0;
	TCNT2 = 0;
	TIMSK2 = 0;

	if(mid != 0){
	    TCCR2A |= (1 << COM2A0)|(1 << WGM21); //init do modo toggle da porta OC2A (PB3)
		TCCR2B |= (1 << CS22)|(1 << CS21); //prescaler = 256
		OCR2A = (uint8_t) mid->ocrValue;
		if(mid->frequency==0) DDRB &= ~(1 << PB3);
		TIMSK2 |= (1 << OCIE2A);
	}
}

void initTimer0(){
	TCCR0A = 0;
	TCCR0B = 0;
	TCNT0 = 0;
	TIMSK0 = 0;

	if(bass != 0){
	    TCCR0A |= (1 << WGM01)|(1 << COM0A0); //init do modo toggle da porta OC0A (PD6)
		TCCR0B |= (1 << CS02); //prescaler = 256
		OCR0A = (uint8_t) bass->ocrValue;
		if(bass->frequency==0) DDRD &= ~(1 << PD6);
		TIMSK0 |= (1 << OCIE0A);
	}
}

int main()
{

	DDRB = (1 << PB1)| (1 << PB3);
	DDRD = (1 << PD6);
	PORTD = (1 << PD2);
	song1 = initializePreludeCMajor();
	treble = song1.firstTreble;
	mid = song1.firstMid;
	bass = song1.firstBass;
	
	cli();

	while((PIND & (1<<PIND2)));
	initTimer0();
	initTimer1();
	initTimer2();
	sei();

	while(1) {}
}


ISR (TIMER1_COMPA_vect) {
    if(flagBuzzer1<treble->durationInPulses) flagBuzzer1++;
	else {
		flagBuzzer1 = 0;
		treble = treble->next;
		if(treble != 0) {
    		DDRB |= (1 << PB1);
			OCR1A = treble->ocrValue;
		} else TIMSK1 = 0;
		if(treble->frequency==0) {
    		DDRB &= ~(1 << PB1);
			OCR1A = treble->ocrValue;
		}
	}
}

ISR (TIMER2_COMPA_vect) {
    if(flagBuzzer2<mid->durationInPulses) flagBuzzer2++;
	else {
		flagBuzzer2 = 0;
		mid = mid->next;
		if(mid != 0) {
    		DDRB |= (1 << PB3);
			OCR2A = (uint8_t) mid->ocrValue;
		} else TIMSK2 = 0;
		if(mid->frequency==0) {
    		DDRB &= ~(1 << PB3);
			OCR2A = (uint8_t) mid->ocrValue;
		}
	}
}

ISR (TIMER0_COMPA_vect) {
    if(flagBuzzer3<bass->durationInPulses) flagBuzzer3++;
	else {
		flagBuzzer3 = 0;
		bass = bass->next;
		if(bass != 0) {
    		DDRD |= (1 << PD6);
			OCR0A = (uint8_t) bass->ocrValue;
		} else TIMSK0 = 0;
		if(bass->frequency==0) {
    		DDRD &= ~(1 << PD6);
			OCR0A = (uint8_t) bass->ocrValue;
		}
	}
}

