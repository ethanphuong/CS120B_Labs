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

enum SM1_STATES { SM1_SMStart, SM1_offRelease, SM1_onPress, SM1_onRelease, SM1_offPress} SM1_STATE;
void Tick_LoHi() {
   switch(SM1_STATE) {
      case SM1_SMStart:
         if (1) {
            SM1_STATE = SM1_offRelease;
         }
         break;
      case SM1_offRelease:
         if ((PINA & 0x01) == 0x01) {
            SM1_STATE = SM1_onPress;
         }
         else if ((PINA & 0x01) != 0x01) {
            SM1_STATE = SM1_offRelease;
         }
         else {
            SM1_STATE = SM1_offRelease;
         }
         break;
      case SM1_onPress:
         if ((PINA & 0x01) == 0x01) {
            SM1_STATE = SM1_onPress;
         }
         else if ((PINA & 0x01) != 0x01) {
            SM1_STATE = SM1_onRelease;
         }
         else {
            SM1_STATE = SM1_onPress;
         }
         break;
      case SM1_onRelease:
         if ((PINA & 0x01) == 0x01) {
            SM1_STATE = SM1_offPress;
         }
         else {
            SM1_STATE = SM1_onRelease;
         }
         break;
      case SM1_offPress:
         if ((PINA & 0x01) != 0x01) {
            SM1_STATE = SM1_offRelease;
         }
         else {
            SM1_STATE = SM1_offPress;
         }
         break;
      default:
         SM1_STATE = SM1_offRelease;
         break;
   }
   switch(SM1_STATE) {
      case SM1_SMStart:
         break;
      case SM1_offRelease:
	 PORTB = 0x01;
         break;
      case SM1_onPress:
	 PORTB = 0x02;
         break;
      case SM1_onRelease:
	 PORTB = 0x02;
         break;
      case SM1_offPress:
	 PORTB = 0x01;
         break;
   }
}
