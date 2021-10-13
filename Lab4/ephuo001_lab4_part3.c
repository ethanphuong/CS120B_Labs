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
    	DDRB = 0xFF; PORTB = 0x00;
	while(1)
	{
		Tick_LoHi();
	}
	return 1;
}

enum SM1_STATES { SM1_SMStart, SM1_Locked, SM1_pressX, SM1_pressY, SM1_pressPound, SM1_releasePound, SM1_YAfterPound, SM1_AutoLock} SM1_STATE;
void Tick_LoHi() { 
	unsigned char PIN_A0 = PINA & 0x01;
	unsigned char PIN_A1 = PINA & 0x02;
	unsigned char PIN_A2 = PINA & 0x04;
	unsigned char locked = PINA & 0x80;
   switch(SM1_STATE) { 
      case SM1_SMStart:
         if (1) {
            	SM1_STATE = SM1_Locked;
         }
         break;
      case SM1_Locked:
         if ((PIN_A0 == 0x01) & !(PIN_A1 == 0x02) & !(PIN_A2 == 0x04)) {
            	SM1_STATE = SM1_pressX;
         }
         else if (!(PIN_A0 == 0x01) & (PIN_A1 == 0x01) & !(PIN_A2 == 0x04)) {
            	SM1_STATE = SM1_pressY;
         }
         else if (!(PIN_A0 == 0x01) & !(PIN_A1 == 0x02) & (PIN_A2 == 0x04)) {
            	SM1_STATE = SM1_pressPound;
         }
	 else if (locked)
	 {
		SM1_STATE = SM1_AutoLock;
	 }
         else {
            	SM1_STATE = SM1_Locked;
         }
         break;
      case SM1_pressX:
	 SM1_STATE = SM1_Locked;
         break;
      case SM1_pressY:
	 SM1_STATE = SM1_Locked;
         break;
      case SM1_pressPound:
	 if (!(PIN_A0 == 0x01) & !(PIN_A1 == 0x02) & !(PIN_A2 == 0x04))
	 {
	 	SM1_STATE = SM1_YAfterPound;
	 }
	 else
	 {
		SM1_STATE = SM1_Locked;
	 }
	 break;
      case SM1_YAfterPound:
	  if (!(PIN_A0 == 0x01) & (PIN_A1 == 0x02) & !(PIN_A2 == 0x04))
	  {
		  SM1_STATE = SM1_YAfterPound;
	  }
	  else
	  {
		  SM1_STATE = SM1_Locked;
	  }
     	 break;
      case SM1_AutoLock:
     	 break;
   }
   switch(SM1_STATE) { 
      case SM1_SMStart:
         PORTB = 0x00;
         break;
      case SM1_Locked:
         
         break;
      case SM1_pressX:
         
         break;
      case SM1_pressY:
	 
	 break;
      case SM1_pressPound:

	 break;
      case SM1_YAfterPound:
         PORTB = 0x01;
         break;
      case SM1_AutoLock:
	 PORTB = 0x00;
	 break;
   }
}
















































