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
    	DDRB = 0xFF; PORTB = 0x07;
	while(1)
	{
		Tick_LoHi();
	}
	return 1;
}

enum SM1_STATES { SM1_SMStart, SM1_nonePresse, SM1_firstPress, SM1_secndPress, SM1_bothPress} SM1_STATE;
void Tick_LoHi() { 
   switch(SM1_STATE) { 
      case SM1_SMStart:
         if (1) {
            SM1_STATE = SM1_nonePresse;
         }
         break;
      case SM1_nonePresse:
         if (((PINA & 0x00) == 0x00) && ((PINA & 0x01) != 0x01)) {
            SM1_STATE = SM1_firstPress;
         }
         else if (((PINA & 0x00) != 0x00) && ((PINA & 0x01) == 0x01)) {
            SM1_STATE = SM1_secndPress;
         }
         else if (((PINA & 0x00) == 0x00) && ((PINA & 0x01) == 0x01)) {
            SM1_STATE = SM1_bothPress;
         }
         else {
            SM1_STATE = SM1_nonePresse;
         }
         break;
      case SM1_firstPress:
         if (((PINA & 0x00) == 0x00) && ((PINA & 0x01) != 0x01)) {
            SM1_STATE = SM1_firstPress;
         }
         else if ((PINA & 0x01) == 0x01) {
            SM1_STATE = SM1_bothPress;
         }
         else {
            SM1_STATE = SM1_firstPress;
         }
         break;
      case SM1_secndPress:
         if (((PINA & 0x00) != 0x00) && ((PINA & 0x01) == 0x01)) {
            SM1_STATE = SM1_secndPress;
         }
         else if ((PINA & 0x00) == 0x00) 
	 {
            SM1_STATE = SM1_bothPress;
         }
         break;
      case SM1_bothPress:
            SM1_STATE = SM1_bothPress;
         break;
      default:
         SM1_STATE = SM1_nonePresse;
         break;
   }
   switch(SM1_STATE) { 
      case SM1_SMStart:
         
         break;
      case SM1_nonePresse:
         PORTB = 0x07;
         break;
      case SM1_firstPress:
         if (PORTB <= 0x08)
	 {
		 PORTB++;
	 }
         break;
      case SM1_secndPress:
	if (PORTB >= 0x01)
	 {
		 PORTB--;
	 }
	break;
      case SM1_bothPress:
         PORTB = 0x00;
         break;
   }
}


































































