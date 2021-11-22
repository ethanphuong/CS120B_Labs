/*	Your Name & E-mail: Ethan Phuong ephuo001@ucr.edu
 *	Lab Section: 23
 *	Assignment: Final Project
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://youtu.be/hHzz5gB_I1g
 */
#include <avr/io.h>
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
	  PORTA = 0x01;
	  //transmit_data_col(0x66);
	  //transmit_data_anti_row(0x3F);
          break;
       case SM_BitTwo:
	  PORTA = 0x02;
	  //transmit_data_col(0x66);
	  //transmit_data_anti_row(0x9F);
          break;
       case SM_BitThree:
	  PORTA = 0x04;
          break;
       default:
	  break;
    }
}

void transmit_data_col(unsigned char data) {
	int i;
	for (i = 0; i < 8; i++) {
		PORTC = 0x08;
		PORTC |= ((data >> i) & 0x01);
		PORTC |= 0x02;
	}
	PORTC |= 0x04;
	PORTC = 0x00;
}

void transmit_data_anti_row(unsigned char data) {
	int j;
	for (j = 0; j < 8; j++) {
		PORTB = 0x08;
		PORTB |= ((data >> j) & 0x01);
		PORTB |= 0x02;
	}
	PORTB |= 0x04;
	PORTB = 0x00;
}

int main(void) {
	DDRA = 0xFF; PORTA = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRB = 0xFF; PORTB = 0x00;

	TimerSet(1000);
	TimerOn();

	SM_STATE = SM1_SMStart;

	while(1) {
		ThreeLEDsSM();
		while (!TimerFlag);
		TimerFlag = 0;
	}
	return 0;
}
