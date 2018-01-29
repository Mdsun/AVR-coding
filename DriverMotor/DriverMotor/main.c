/*
 * conveyer_motor_atmega328p.c
 *
 * Created: 11/19/2017 2:41:17 PM
 * Author : HP USER
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>



int main(void)
{
	DDRB=0b00000011;
	/* Replace with your application code */
	while (1)
	{
		if(PINB & (1<<PINB3))
		{
			PORTB=(1<<PORTB1);
			PORTB &=~(1<<PORTB0);
			
			
		}
		
		else if(PINB & (1<<PINB4))
		
		{
			PORTB&=~(1<<PORTB1);
			PORTB =(1<<PORTB0);
			
			
			
		}
	}
}
