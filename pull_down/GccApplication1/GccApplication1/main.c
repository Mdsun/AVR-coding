/*
 * GccApplication1.c
 *
 * Created: 10/18/2017 10:16:35 PM
 * Author : Amin
 */ 

#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h>


int main(void)
{
	DDRC &= (0<<PD0);
	DDRD |= (1<<PD0);
	/* Replace with your application code */
	while (1)
	{
		 uint8_t input = 0;
		 input = PINC & 0x01;
		if( input == 0x00){
			
			PORTD &= (1<<PD0);
			if(PORTD == 0x01){
				PORTD = 0x00;
			}
		}
		else{
			PORTD |= (1<<PD0);
		}
	}
}

