/*	Your Name & E-mail: Ethan Phuong ephuo001@ucr.edu
 *	Lab Section: 23
 *	Assignment: Lab 2  Exercise 1 
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: <URL>
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
int main(void) {
	DDRA = 0x00; PORTA = 0xFF; 
	DDRB = 0x00; PORTB = 0xFF;
    DDRC = 0x00; PORTC = 0xFF;
	DDRD = 0xFF; PORTD = 0x00;	
	unsigned char tmpA = 0x00;
	unsigned char tmpB = 0x00;
	unsigned char tmpC = 0x00;
	unsigned char tmpD = 0x00;
	unsigned char total = 0x00;
	while(1) {
		
		tmpA = tmpA & 0x00;
		tmpB = PINA;
		tmpC = PINB;
		tmpD = PINC;	
		total = tmpB + tmpC + tmpD;
		
		if (total > 0x8C) 
		{
			tmpA = (tmpA | 0x01);							 
		}
		
		if ((tmpD - tmpB) > 0x50)
		{
			tmpA = (tmpA | 0x02);
		}
		else if ((tmpB - tmpD) > 0x50)
		{
			tmpA = (tmpA | 0x02);
		}

		tmpA = ((total >> 2) & 0x3C) | tmpA;
		
	PORTD = tmpA;	
	}
	return 0;
}

