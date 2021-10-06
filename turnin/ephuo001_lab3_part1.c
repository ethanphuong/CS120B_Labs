/*	Your Name & E-mail: Ethan Phuong ephuo001@ucr.edu
 *	Lab Section: 23
 *	Assignment: Lab 3  Exercise 1 
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
    	DDRC = 0xFF; PORTC = 0x00;	
	unsigned char tmpA = 0x00;
	unsigned char tmpB = 0x00;
	unsigned char total = 0x00;
	while(1) {
		
		tmpA = PINA;
		tmpB = PINB;
		total = 0x00;
		
		if ((tmpA & 0x01) == 0x01)
		{
			total++;
		}
		if ((tmpB & 0x01) == 0x01)
		{
			total++;
		}
		if ((tmpA & 0x02) == 0x02)
		{
			total++;
		}
		if ((tmpB & 0x02) == 0x02)
		{
			total++;
		}
		if ((tmpA & 0x04) == 0x04)
		{
			total++;
		}
		if ((tmpB & 0x04) == 0x04)
		{
			total++;
		}
		if ((tmpA & 0x08) == 0x08)
		{
			total++;
		}
		if ((tmpB & 0x08) == 0x08)
		{
			total++;
		}
		if ((tmpA & 0x10) == 0x10)
		{
			total++;
		}
		if ((tmpB & 0x10) == 0x10)
		{
			total++;
		}
		if ((tmpA & 0x20) == 0x20)
		{
			total++;
		}
		if ((tmpB & 0x20) == 0x20)
		{
			total++;
		}
		if ((tmpA & 0x40) == 0x40)
		{
			total++;
		}
		if ((tmpB & 0x40) == 0x40)
		{
			total++;
		}
		if ((tmpA & 0x80) == 0x80)
		{
			total++;
		}
		if ((tmpB & 0x80) == 0x80)
		{
			total++;
		}
			
	PORTC = total;	
	}
	return 0;
}
