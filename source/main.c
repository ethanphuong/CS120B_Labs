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
    /* Insert DDR and PORT initializations */
    /* Insert your solution below */

    while (1) 
    {
      Tick_LoHi();
    }
    return 1;
}

enum SM1_STATES {SM1_SMStart, SM1_zeroLight, SM1_oneLight, SM1_twoLight, SM1_threeLight, SM1_fourLight, SM1_fiveLight, SM1_sixLight, SM1_Release1, SM1_Release2, SM1_Release3, SM1_Release4, SM1_Release5} SM1_STATE;
void Tick_LoHi() {
  switch(SM1_STATE) { 
    case SM1_SMStart:
      if (1) {
	 SM1_STATE = SM1_zeroLight;
      }
      break;
    case SM1_zeroLight:
      if (~PINA & 0x01) {
      	SM1_STATE = SM1_oneLight;
      } 
      else {
      	SM1_STATE = SM1_zeroLight;
      }
      break;
    case SM1_oneLight:
      if (!(~PINA & 0x01)) {
        SM1_STATE = SM1_Release1;
      } 
      else {
        SM1_STATE = SM1_oneLight;
      }
      break;
    case SM1_twoLight:
      if (!(~PINA & 0x01)) {
        SM1_STATE = SM1_Release2;
      } 
      else {
      	SM1_STATE = SM1_twoLight;
      }
      break;
    case SM1_threeLight:
      if (!(~PINA & 0x01)) {
        SM1_STATE = SM1_Release3;
      } 
      else {
        SM1_STATE = SM1_threeLight;
      }
      break;
    case SM1_fourLight:
      if (!(~PINA & 0x01)) {
        SM1_STATE = SM1_Release4;
      } else {
        SM1_STATE = SM1_fourLight;
      }
      break;
    case SM1_fiveLight:
      if (!(~PINA & 0x01)) {
        SM1_STATE = SM1_Release5;
      } else {
        SM1_STATE = SM1_fiveLight;
      }
      break;
    case SM1_sixLight:
      if (!(~PINA & 0x01)) {
        SM1_STATE = SM1_zeroLight;
      } 
      else {
        SM1_STATE = SM1_sixLight;
      }
      break;
    case SM1_Release1:
      if (~PINA & 0x01) {
        SM1_STATE = SM1_twoLight;
      } 
      else {
      	SM1_STATE = SM1_Release1;
      }
      break;
    case SM1_Release2:
      if (~PINA & 0x01) {
        SM1_STATE = SM1_threeLight;
      } 
      else {
        SM1_STATE = SM1_Release2;
      }
      break;
    case SM1_Release3:
      if (~PINA & 0x01) {
        SM1_STATE = SM1_fourLight;
      } 
      else {
        SM1_STATE = SM1_Release3;
      }
      break;
    case SM1_Release4:
      if (~PINA & 0x01) {
        SM1_STATE = SM1_fiveLight;
      } else {
        SM1_STATE = SM1_Release4;
      }
      break;
    case SM1_Release5:
      if (~PINA & 0x01) {
        SM1_STATE = SM1_sixLight;
      } 
      else {
        SM1_STATE = SM1_Release5;
      }
      break;
  }
  switch (SM1_SMStart) 
  {
    case SM1_SMStart:
      break;
    case SM1_zeroLight:
      break;
    case SM1_oneLight:
      break;
    case SM1_twoLight:
      break;
    case SM1_threeLight:
      break;
    case SM1_fourLight:
      break;
    case SM1_fiveLight:
      break;
    case SM1_sixLight:
      break;
    case SM1_Release1:
      PORTB = 0x01;
      break;
    case SM1_Release2:
      PORTB = 0x03;
      break;
    case SM1_Release3:
      PORTB = 0x07;
      break;
    case SM1_Release4:
      PORTB = 0x0F;
      break;
    case SM1_Release5:
      PORTB = 0x3F;
      break;
  }
}


