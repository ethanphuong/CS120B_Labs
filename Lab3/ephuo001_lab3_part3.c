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
    	DDRC = 0xFF; PORTC = 0x00;	
	unsigned char tmpA = 0x00;
	unsigned char total = 0x00;
	while(1) {
		
		tmpA = PINA;
		total = 0x00;
		
		if ((tmpA & 0x00) == 0x00)
		{
			total = (total | 0x40);
		}
		if (((tmpA & 0x01) == 0x01) || ((tmpA & 0x02) == 0x02))
		{
			total = (total | 0x60);
		}
		if (((tmpA & 0x03) == 0x03) || ((tmpA & 0x04) == 0x04))
		{
			total = (total | 0x70);
		}
		if (((tmpA & 0x05) == 0x05) || ((tmpA & 0x06) == 0x06))
		{
			total = (total | 0x38);
			total = (total & 0x3F);
		}
		if (((tmpA & 0x07) == 0x07) || ((tmpA & 0x08) == 0x08) || ((tmpA & 0x09) == 0x09))
		{
			total = (total | 0x3C);
			total = (total & 0x3F);
		}
		if (((tmpA & 0x0A) == 0x0A) || ((tmpA & 0x0B) == 0x0B) || ((tmpA & 0x0C) == 0x0C))
		{
			total = (total | 0x3E);
			total = (total & 0x3F);
		}
		if (((tmpA & 0x0D) == 0x0D) || ((tmpA & 0x0E) == 0x0E) || ((tmpA & 0x0F) == 0x0F))
		{
			total = (total | 0x3F);
			total = (total & 0x3F);
		}
		if (((tmpA & 0x10) == 0x10) && ((tmpA & 0x20) == 0x20))
		{
			if ((tmpA & 0x40) != 0x40)
			{
				total = (total | 0x80);
			}
		}
	PORTC = total;	
	}
	return 0;
}
