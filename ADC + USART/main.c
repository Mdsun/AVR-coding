/*
 * ADC.c
 *
 * Created: 12/10/2016 11:48:32 PM
 * Author : Krishna Ray
 */ 
#define F_CPU 16000000UL  // 16 MHz
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//Define Baud
#define USART_BAUDRATE 19200
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

#pragma region Global Variables
volatile uint8_t value = 0;// volatile so both main and RX interrupt can use it
volatile uint8_t newData = 0;
volatile uint8_t adcHData = 0;
volatile uint8_t adcLData = 0;
#pragma endregion

#pragma region Function
ISR(USART_RX_vect)
{
	value = UDR0;
	newData = 1;
}
void USART_Init(void)
{
	// Set baud rate
	UBRR0L = BAUD_PRESCALE;//lower 8-bits into the the UBRRL register
	UBRR0H = (BAUD_PRESCALE >> 8); //upper 8-bits into the UBRRH register
	UCSR0B = ((1<<TXEN0)|(1<<RXEN0) | (1<<RXCIE0));//enable tx and Rx. Receive interrupt
	
	/* Default frame format is 8 data bits, no parity, 1 stop bit to change use UCSRC, see AVR datasheet*/
}
void USART_SendByte(uint8_t data){

	// Wait until last byte has been transmitted
	while((UCSR0A &(1<<UDRE0)) == 0);

	// Transmit data
	UDR0 = data;
}
void USART_Sends (const char *s)
{
	do
	{
		USART_SendByte (*s);		//send one char of string
	}
	while (*s++);			//next char of string until end of string sign
}
void USART_Sendi(const uint16_t var) {
	char cache[7];
	USART_Sends (utoa(var, cache, 10));
}
void InitADC()
{
	// Select Vref=AVcc
	ADMUX |= (1<<REFS0);
	//set prescaller to 128 and enable ADC
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);
}
uint16_t ReadADC(uint8_t ADCchannel)
{
	//select ADC channel with safety mask
	ADMUX = (ADMUX & 0xF0) | (ADCchannel & 0x0F);
	//single conversion mode
	ADCSRA |= (1<<ADSC);
	// wait until ADC conversion is complete
	while( ADCSRA & (1<<ADSC) );
	adcHData = ADCH;
	adcLData = ADCL;
	return ADC;
}
void LEDInit(void){
	DDRB = 0x20;//B5 Output
	PORTB = 0x00;//All off
}
#pragma endregion 

int main(void)
{
	LEDInit();//Init LED
	//initialize ADC
	InitADC();
	uint16_t adc_value; // Variable to hold ADC result
	USART_Init();
	sei();
#pragma region USART Send "IOT-Army"
	USART_SendByte(0x49);
	USART_SendByte('O');
	USART_SendByte(0b01010100);
	USART_SendByte(45);
	USART_SendByte(0x41);
	USART_SendByte('R');
	USART_SendByte(0x4D);
	USART_SendByte(0b01011001);
#pragma endregion
    while (1) 
    {
		adc_value = ReadADC(0);
		//uint8_t adcH = adc_value >> 8;
		//uint8_t adcL = adc_value & 0x0F;
		if (adc_value >= 512)
			PORTB |= (1<<PORTB5);
		else
			PORTB &=~ (1<<PORTB5);
		//USART_SendByte(ADCL);
#pragma region Send Adc Data through USART
		USART_SendByte(0x0D);
		USART_SendByte(0x0A);
		USART_Sendi(adc_value);
#pragma endregion
		_delay_ms(200);
    }
}