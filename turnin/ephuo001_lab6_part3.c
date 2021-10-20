/*	Your Name & E-mail: Ethan Phuong ephuo001@ucr.edu
 *	Lab Section: 23
 *	Assignment: Lab 6  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://youtu.be/GiFspyPORF8
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {

	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
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
         if (((~PINA & 0x01) == 0x01) && ((~PINA & 0x02) != 0x02)) {
            	SM1_STATE = SM1_firstPress;
         }
         else if (((~PINA & 0x01) != 0x01) && ((~PINA & 0x02) == 0x02)) {
            	SM1_STATE = SM1_secondPress;
         }
         else if ((~PINA & 0x03) == 0x03) {
            	SM1_STATE = SM1_bothPress;
         }
         else {
            	SM1_STATE = SM1_nonePressed;
         }
         break;
      case SM1_first:
         if (((~PINA & 0x01) == 0x01) && ((~PINA & 0x02) != 0x02)) {
            	SM1_STATE = SM1_firstPress;
         }
         else if ((~PINA & 0x02) == 0x02) {
            	SM1_STATE = SM1_bothPress;
         }
         else {
            	SM1_STATE = SM1_nonePressed;
         }
         break;
      case SM1_second:
        if (((~PINA & 0x01) != 0x01) && ((~PINA & 0x02) == 0x02)) {
        	SM1_STATE = SM1_secondPress;
        }
        else if ((~PINA & 0x01) == 0x01) 
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
	if ((~PINA & 0x03) == 0x03)
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
         PORTB = 0x07;
         break;
      case SM1_nonePressed:
         
         break;
      case SM1_first:
         
         break;
      case SM1_firstPress:
	 if (PORTB <= 0x08)
	 {
	 	PORTB++;
	 }
      case SM1_second:
	
	break;
      case SM1_secondPress:
	if (PORTB > 0x01)
	 {
		 PORTB--;
	 }
	 break;
      case SM1_bothPress:
         PORTB = 0x00;
         break;
   }
}

void main() {

	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	TimerSet(100);
	TimerOn();

	SM1_STATE = SM1_SMStart;
	
	while(1) {
		Tick_LoHi();
		while (!TimerFlag);
		TimerFlag = 0;
	}
}

