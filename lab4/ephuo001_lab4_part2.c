
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
    	DDRC = 0xFF; PORTC = 0x07;
	while(1)
	{
		Tick_LoHi();
	}
	return 1;
}

enum SM1_STATES { SM1_SMStart, SM1_nonePressed, SM1_first, SM1_second, SM1_bothPress, SM1_firstPress, SM1_secondPress} SM1_STATE;
void Tick_LoHi() { 
   switch(SM1_STATE) { 
      case SM1_SMStart:
         if (1) {
            	SM1_STATE = SM1_nonePressed;
         }
         break;
      case SM1_nonePressed:
         if (((PINA & 0x01) == 0x01) && ((PINA & 0x02) != 0x02)) {
            	SM1_STATE = SM1_firstPress;
         }
         else if (((PINA & 0x01) != 0x01) && ((PINA & 0x02) == 0x02)) {
            	SM1_STATE = SM1_secondPress;
         }
         else if ((PINA & 0x03) == 0x03) {
            	SM1_STATE = SM1_bothPress;
         }
         else {
            	SM1_STATE = SM1_nonePressed;
         }
         break;
      case SM1_first:
         if (((PINA & 0x01) == 0x01) && ((PINA & 0x02) != 0x02)) {
            	SM1_STATE = SM1_first;
         }
         else if ((PINA & 0x02) == 0x02) {
            	SM1_STATE = SM1_bothPress;
         }
         else {
            	SM1_STATE = SM1_nonePressed;
         }
         break;
      case SM1_second:
        if (((PINA & 0x01) != 0x01) && ((PINA & 0x02) == 0x02)) {
        	SM1_STATE = SM1_second;
        }
        else if ((PINA & 0x01) == 0x01) 
	{
            	SM1_STATE = SM1_bothPress;
	}
	else {
		SM1_STATE = SM1_nonePressed;
	}
        break;
	case SM1_secondPress:
		SM1_STATE = SM1_second;
		break;
	case SM1_firstPress:
		SM1_STATE = SM1_first;
		break;
      	case SM1_bothPress:
	if ((PINA & 0x03) == 0x03)
	{
            	SM1_STATE = SM1_bothPress;
	}
	else
	{
		SM1_STATE = SM1_nonePressed;
	}
        break;
   }
   switch(SM1_STATE) { 
      case SM1_SMStart:
         PORTC = 0x07;
         break;
      case SM1_nonePressed:
         
         break;
      case SM1_first:
         
         break;
      case SM1_firstPress:
	 if (PORTC <= 0x08)
	 {
	 	PORTC++;
	 }
      case SM1_second:
	
	break;
      case SM1_secondPress:
	if (PORTC > 0x01)
	 {
		 PORTC--;
	 }
	 break;
      case SM1_bothPress:
         PORTC = 0x00;
         break;
   }
}































































