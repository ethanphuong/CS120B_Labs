/*	Your Name & E-mail: Ethan Phuong ephuo001@ucr.edu
 *	Lab Section: 23
 *	Assignment: Lab 6  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://youtu.be/_8FW3p1dWDg
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

enum SM_STATES {SM1_SMStart, SM_FirstLed, SM_SecondLed, SM_ThirdLed} SM_STATE;
void Tick_LoHi() {
	switch(SM_STATE) {
	   case SM1_SMStart:
	      SM_STATE = SM_FirstLed;
	      break;
	   case SM_FirstLed:
	      SM_STATE = SM_SecondLed;
	      break;
	   case SM_SecondLed:
	      SM_STATE = SM_ThirdLed;
	      break;
	   case SM_ThirdLed:
	      SM_STATE = SM_FirstLed;
	      break;
	   default:
	      SM_STATE = SM1_SMStart;
	      break;
	}

	switch(SM_STATE) {
	   case SM1_SMStart:
	      PORTB = 0x00;
	      break;
	   case SM_FirstLed:
	      PORTB = 0x01;
	      break;
	   case SM_SecondLed:
	      PORTB = 0x02;
	      break;
	   case SM_ThirdLed:
	      PORTB = 0x04;
	      break;
	   default:
	      break;
	}
}

void main() {
	DDRB = 0xFF; PORTB = 0x00;
	
	TimerSet(1000);
	TimerOn();

	SM_STATE = SM1_SMStart;
	
	while(1) {
		Tick_LoHi();
		while (!TimerFlag);
		TimerFlag = 0;
	}
}

