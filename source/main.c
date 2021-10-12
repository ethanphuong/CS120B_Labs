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
        DDRB = 0xFF; PORTB = 0x00;
	while(1) {
		PORTB = 0x0F;	
	}
	return 0;
}





























