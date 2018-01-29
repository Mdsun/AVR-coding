/*
 * TimeCount.c
 *
 * Created: 10/29/2017 3:16:17 PM
 * Author : novel
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t timeVariable =0;
ISR(TIMER0_OVF_vect){
	timeVariable++;
	if(timeVariable >= 125){
		timeVariable =0;
		PORTB ^=(1<< PORTB5);
	}	
}
int main(void)
{
	DDRB = 0x20;
	TCCR0B |= (1<<CS00);
	TIFR0 = 1<<TOV0;
	TIMSK0 =1<<TOIE0;
	sei();
    /* Replace with your application code */
    while (1) 
    {
    }
}

