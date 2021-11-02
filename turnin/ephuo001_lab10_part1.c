/*	Your Name & E-mail: Ethan Phuong ephuo001@ucr.edu
 *	Lab Section: 23
 *	Assignment: Lab 10  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://youtu.be/77ZsV61rQ4Y
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

void set_PWM(double frequency) {
	static double current_frequency;
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; }
		else { TCCR3B |= 0x03; }
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		else if (frequency > 31250) { OCR3A = 0x0000; }
		else { OCR3A = (short) (8000000 / (128 * frequency)) -1; }
		TCNT3 = 0;
		current_frequency = frequency;
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

enum SM_STATES {SM1_SMStart, SM_BitOne, SM_BitTwo, SM_BitThree} SM_STATE;
unsigned char threeLEDs = 0x00;
void ThreeLEDsSM() {
    switch (SM_STATE) {
       case SM1_SMStart:
          SM_STATE = SM_BitOne;
	  break;
       case SM_BitOne:
	  SM_STATE = SM_BitTwo;
	  break;
       case SM_BitTwo:
	  SM_STATE = SM_BitThree;
	  break;
       case SM_BitThree:
	  SM_STATE = SM_BitOne;
	  break;
       default:
	  SM_STATE = SM1_SMStart;
	  break;
    }
    switch (SM_STATE) {
       case SM1_SMStart:
          break;
       case SM_BitOne:
	  threeLEDs = 0x01;
          break;
       case SM_BitTwo:
	  threeLEDs = 0x02;
          break;
       case SM_BitThree:
	  threeLEDs = 0x04;
          break;
       default:
	  break;
    }
}

enum SM2_STATES {SM2_SMStart, SM2_BitThreeOff, SM2_BitThreeOn} SM2_STATE;
unsigned char blinkingLED = 0x00;
void BlinkingLEDSM() {
    switch (SM2_STATE) {
       case SM2_SMStart:
          SM2_STATE = SM2_BitThreeOn;
	  break;
       case SM2_BitThreeOn:
	  SM2_STATE = SM2_BitThreeOff;
	  break;
       case SM2_BitThreeOff:
	  SM2_STATE = SM2_BitThreeOn;
	  break;
       default:
	  SM2_STATE = SM2_SMStart;
	  break;
    }
    switch (SM2_STATE) {
       case SM2_SMStart:
          break;
       case SM2_BitThreeOn:
	  blinkingLED = 0x08;
          break;
       case SM2_BitThreeOff:
	  blinkingLED = 0x00;
          break;
       default:
	  break;
    }
}

enum SM3_STATES {SM3_SMStart, SM3_SMOn} SM3_STATE;
void CombineLEDsSM() {
    switch (SM3_STATE) {
       case SM3_SMStart:
          SM3_STATE = SM3_SMOn;
	  break;
       case SM3_SMOn:
	  SM3_STATE = SM3_SMOn;
	  break;
       default:
	  SM3_STATE = SM3_SMStart;
	  break;
    }
    switch (SM3_STATE) {
       case SM3_SMStart:
          break;
       case SM3_SMOn:
	  PORTB = (threeLEDs | blinkingLED);
          break;
       default:
	  break;
    }
}
void main() {

	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	TimerSet(1000);
	TimerOn();

	SM_STATE = SM1_SMStart;
	SM2_STATE = SM2_SMStart;
	SM3_STATE = SM3_SMStart;

	while(1) {
		ThreeLEDsSM();
		BlinkingLEDSM();
		CombineLEDsSM();
		while (!TimerFlag);	
		TimerFlag = 0;
	}
}
