/*	Your Name & E-mail: Ethan Phuong ephuo001@ucr.edu
 *	Lab Section: 23
 *	Assignment: Lab 7  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://youtu.be/Hn3Tc_939TE
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned short my_short;
unsigned char my_char;

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

void main() {

	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	ADC_init();

	while(1) {
		my_short = ADC;
		my_char = (char)my_short;
		if (my_char <= (0xFF/8))
		{
			PORTB = 0x01;
		}
		else if ((my_char) <= (2*(0xFF/8)))
		{
			PORTB = 0x03;
		}
		else if ((my_char) <= (3*(0xFF/8)))
		{
			PORTB = 0x07;
		}
		else if ((my_char) <= (4*(0xFF/8)))
		{
			PORTB = 0x0F;
		}
		else if ((my_char) <= (5*(0xFF/8)))
		{
			PORTB = 0x1F;
		}
		else if ((my_char) <= (6*(0xFF/8)))
		{
			PORTB = 0x3F;
		}
		else if ((my_char) <= (7*(0xFF/8)))
		{
			PORTB = 0x7F;
		}
		else if ((my_char) <= (8*(0xFF/8)))
		{
			PORTB = 0xFF;
		}
	}
	return 0;
}
