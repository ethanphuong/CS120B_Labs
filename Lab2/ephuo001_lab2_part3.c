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
	DDRB = 0xFF; PORTB = 0x00; 
	unsigned char tmpA = 0x00;
	
	unsigned char cntavail = 0x03;
	while(1) {
		
		tmpA = PINA & 0x0F;
		
		if (tmpA == 0xF) 
		{
			cntavail = 0x80;							 
		}
		else if (tmpA == 0x00)
		{
			cntavail = 0x04;
		}	
		else if ((tmpA == 0x0E) || (tmpA == 0x0D) || (tmpA == 0x0B) || (tmpA == 0x07))
		{
			cntavail = 0x01; 
							 
		}
		else if ((tmpA == 0x08) || (tmpA == 0x04) || (tmpA == 0x02) || (tmpA == 0x01))
		{
			cntavail = 0x03;
		}
		else if ((tmpA == 0x0C) || (tmpA == 0x0A) || (tmpA == 0x09) || (tmpA == 0x05) || (tmpA == 0x03) || (tmpA = 0x06))
		{
			cntavail = 0x02;
		}
	
	PORTC = cntavail;	
	}
	return 0;
}

