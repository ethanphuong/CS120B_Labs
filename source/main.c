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
    	DDRC = 0xFF; PORTB = 0x00;
	while(1)
	{
		Tick_LoHi();
	}
	return 1;
}

enum SM1_STATES { SM1_SMStart, SM1_oneLight, SM1_twoLight, SM1_threeLight, SM1_fourLight, SM1_fiveLight, SM1_sixLight, SM1_WAIT2, SM1_WAIT3, SM1_WAIT4, SM1_WAIT5, SM1_WAIT6} SM1_STATE;
void Tick_LoHi() { 
   switch(SM1_STATE) { 
      case SM1_SMStart:
         if (1) {
            SM1_STATE = SM1_oneLight;
         }
         break;
      case SM1_oneLight:
         if ((~PINA & 0x01) == 0x01) {
            SM1_STATE = SM1_WAIT2;
         }
         else {
            SM1_STATE = SM1_oneLight;
         }
         break;
      case SM1_twoLight:
         if ((~PINA & 0x01) == 0x01) {
            SM1_STATE = SM1_WAIT3;
         }
	 else {
	    SM1_STATE = SM1_twoLight;
	 }
         break;
      case SM1_threeLight:
         if ((~PINA & 0x01) == 0x01) {
            SM1_STATE = SM1_WAIT4;
         }
         else {
            SM1_STATE = SM1_threeLight;
         }
         break;
      case SM1_fourLight:
         if ((~PINA & 0x01) == 0x01) {
            SM1_STATE = SM1_WAIT5;
         }
         else {
            SM1_STATE = SM1_fourLight;
         }
         break;
      case SM1_fiveLight:
         if ((~PINA & 0x01) == 0x01) {
            SM1_STATE = SM1_WAIT6;
         }
	 else {
	    SM1_STATE = SM1_fiveLight;
	 }
         break;
      case SM1_sixLight:
         if ((~PINA & 0x01) == 0x01) {
            SM1_STATE = SM1_oneLight;
         }
	 else {
	    SM1_STATE = SM1_sixLight;
	 }
         break;
      case SM1_WAIT2:
	 if ((~PINA & 0x01) != 0x01) {
	   SM1_STATE = SM1_twoLight;
	 }
      case SM1_WAIT3:
	 if ((~PINA & 0x01) != 0x01) {
	   SM1_STATE = SM1_threeLight;
	 }
      case SM1_WAIT4:
	 if ((~PINA & 0x01) != 0x01) {
	   SM1_STATE = SM1_fourLight;
	 }
      case SM1_WAIT5:
	 if ((~PINA & 0x01) != 0x01) {
	   SM1_STATE = SM1_fiveLight;
	 }
      case SM1_WAIT6:
	 if ((~PINA & 0x01) != 0x01) {
	   SM1_STATE = SM1_sixLight;
	 }
      default:
         SM1_STATE = SM1_oneLight;
         break;
   }
   switch(SM1_STATE) { 
      case SM1_SMStart:
         
         break;
      case SM1_oneLight:
         PORTB = 0x01;
         break;
      case SM1_twoLight:
         PORTB = 0x03;
         break;
      case SM1_threeLight:
         PORTB = 0x07;
         break;
      case SM1_fourLight:
         PORTB = 0x0F;
         break;
      case SM1_fiveLight:
         PORTB = 0x1F;
         break;
      case SM1_sixLight:
         PORTB = 0x3F;
         break;
      case SM1_WAIT2:
	 break;
      case SM1_WAIT3:
	 break;
      case SM1_WAIT4:
	 break;
      case SM1_WAIT5:
	 break;
      case SM1_WAIT6:
	 break;
   }
}

