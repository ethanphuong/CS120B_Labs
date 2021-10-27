/*	Your Name & E-mail: Ethan Phuong ephuo001@ucr.edu
 *	Lab Section: 23
 *	Assignment: Lab 7  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://youtu.be/Hn3Tc_939TE
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

enum SM_STATES {SM1_SMStart, SM_FirstLed, SM_SecondLed, SM_ThirdLed, SM_Stop} SM_STATE;

unsigned char whichLight = 0;
unsigned char afterThree = 0;
unsigned char count = 0;

void Tick_LoHi() {
	LCD_ClearScreen();

	if (count == 4)
	{
	   LCD_DisplayString(1, "WINNER");
	}
	else
	{
	   LCD_WriteData((5 + count) + '0');
	}
	switch(SM_STATE) {
	   case SM1_SMStart:
	      SM_STATE = SM_FirstLed;
	      break;
	   case SM_FirstLed:
	      if (~PINA & 0x01) {
		 count--;
		 SM_STATE = SM_Stop;
	      }
	      else {
	         SM_STATE = SM_SecondLed;
	      }
	      break;
	   case SM_SecondLed:
	      if (~PINA & 0x01) {
		 count++;
		 SM_STATE = SM_Stop;
	      }
	      else if (afterThree != 0)
	      {
		 afterThree = 0;
		 SM_STATE = SM_FirstLed;
	      }
	      else
	      {
	         SM_STATE = SM_ThirdLed;
	      }
	      break;
	   case SM_ThirdLed:
	      if (~PINA & 0x01) {
		 count--;
		 SM_STATE = SM_Stop;
	      }
	      else
	      {
		 afterThree = 1;
	         SM_STATE = SM_SecondLed;
	      }
	      break;
	   case SM_Stop:
	      if (!(~PINA & 0x01))
	      {
	         if (whichLight == 0x01)
		 {
	 	    PORTB = 0x01;
		 }
		 else if (whichLight == 0x02)
		 {
		    PORTB = 0x02;
		 }
		 else
		 {
		    PORTB = 0x04;
		 }
	      }
	      else
	      {
	         SM_STATE = SM1_SMStart;
	      }
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
	      whichLight = 0x01;
	      break;
	   case SM_SecondLed:
	      PORTB = 0x02;
	      whichLight = 0x02;
	      break;
	   case SM_ThirdLed:
	      PORTB = 0x04;
	      whichLight = 0x03;
	      break;
	   case SM_Stop:
	      break;
	   default:
	      break;
	}
}

void main() {

	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	TimerSet(250);
	TimerOn();

	SM_STATE = SM1_SMStart;
	
	while(1) {
		Tick_LoHi();
		while (!TimerFlag);
		TimerFlag = 0;
	}
}

