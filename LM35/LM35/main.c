/*
 * GccApplication1.c
 *
 * Created: 11/20/2017 12:03:32 PM
 * Author : HP USER
 */ 
#include <avr/io.h>
#define F_CPU 16000000UL//
#include <util/delay.h>
#include <stdlib.h>
#define BAUDRATE 19200
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

uint16_t adc_value;            //Variable used to store the value read from the ADC
void adc_init(void);            //Function to initialize/configure the ADC
uint16_t read_adc(uint8_t channel);    //Function to read an arbitrary analogic channel/pin
void USART_init(void);            //Function to initialize and configure the USART/serial
void USART_send( unsigned char data);    //Function that sends a char over the serial port
void USART_putstring(char* StringPtr);    //Function that sends a string over the serial port

int main(void){
	adc_init();        //Setup the ADC
	USART_init();        //Setup the USART
	DDRC = 0xff;
	
	while(1){
		uint16_t x= read_adc(0);
		uint16_t result = x/2.05333; //as refernece voltage 5V=5000
		
		char buffer[5];
		utoa(result,buffer,10);
		USART_putstring("value is :");
		USART_putstring(buffer);
		USART_send(13);
		_delay_ms(100);
				
	}
	
	return 0;
}

void adc_init(void){
	ADCSRA = ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));    //16Mhz/128 = 125Khz the ADC reference clock
	ADMUX = (1<<REFS0);                //Voltage reference from Avcc (5v)
	ADCSRA |= (1<<ADEN);                //Turn on ADC
	ADCSRA |= (1<<ADSC);                //Do an initial conversion because this one is the slowest and to ensure that everything is up and running
}

uint16_t read_adc(uint8_t channel){
	ADMUX &= 0xF0;                    //Clear the older channel that was read
	ADMUX |= channel;                //Defines the new ADC channel to be read
	ADCSRA |= (1<<ADSC);                //Starts a new conversion
	while(ADCSRA & (1<<ADSC));            //Wait until the conversion is done
	return ADC;                    //Returns the ADC value of the chosen channel
}

void USART_init(void){
	
	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (3<<UCSZ00);
}

void USART_send( unsigned char data){
	
	while(!(UCSR0A & (1<<UDRE0)));
	//printf("value is %u ",data);
	//UDR0 = '1';
	UDR0 = data;
	
	
	
}

void USART_putstring(char* StringPtr){
	
	while(*StringPtr != 0x00){
		
		USART_send(*StringPtr);
		StringPtr++;
	}
	
}

