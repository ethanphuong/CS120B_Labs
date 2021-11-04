/*	Your Name & E-mail: Ethan Phuong ephuo001@ucr.edu
 *	Lab Section: 23
 *	Assignment: Lab 10  Exercise 4
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://youtu.be/Fwu2PwC_K-w
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

enum SM4_STATES {SM4_SMStart, SM4_On, SM4_Off, SM4_Up, SM4_Down} SM4_STATE;
unsigned char speakerSound = 0x00;
unsigned char count = 0x00;
unsigned char freq = 0x00;
void SpeakerSM() {
    switch (SM4_STATE) {
       case SM4_SMStart:
          SM4_STATE = SM4_Off;
	  break;
       case SM4_Off:
	  if (((~PINA & 0x04) == 0x04) && (count > freq))
	  {
	     SM4_STATE = SM4_On;
	  }
	  else if ((~PINA & 0x02) == 0x02)
	  {
	     SM4_STATE = SM4_Up;
	  }
	  else if ((~PINA & 0x01) == 0x01)
	  {
	     SM4_STATE = SM4_Down;
	  }
	  else
	  {	  
	     count++;
	     SM4_STATE = SM4_Off;
	  }
	  break;
       case SM4_On:
	  count = 0;
	  SM4_STATE = SM4_Off;
	  break;
       case SM4_Up:
	  if ((~PINA & 0x02) != 0x02)
	  {
	     if (freq > 0)
	     { 
	        freq--;
	     }
	  }
	  SM4_STATE = SM4_Off;
	  break;
       case SM4_Down:
	  if ((~PINA & 0x01) != 0x01)
	  {
	     if (freq < 10)
	     {
	        freq++;
	     }
	  }
	  SM4_STATE = SM4_Off;
	  break;
       default:
	  SM4_STATE = SM4_SMStart;
	  break;
    }
    switch (SM4_STATE) {
       case SM4_SMStart:
          break;
       case SM4_Off:
	  speakerSound = 0x00;
          break;
       case SM4_On:
	  speakerSound = 0x10;
          break;
       case SM4_Up:
	  break;
       case SM4_Down:
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
	  PORTB = (threeLEDs | blinkingLED) | speakerSound;
          break;
       default:
	  break;
    }
}
void main() {

	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	unsigned long i = 0x00;
	unsigned long j = 0x00;
	unsigned long k = 0x00;
	unsigned long time = 0x01;

	TimerSet(1);
	TimerOn();

	SM_STATE = SM1_SMStart;
	SM2_STATE = SM2_SMStart;
	SM3_STATE = SM3_SMStart;
	SM4_STATE = SM4_SMStart;

	while(1) {
		if ((i % 300) == 0x00)
		{
		   ThreeLEDsSM();
		   i = 0x00;
		}

		if ((j % 1000) == 0x00)
		{
		   BlinkingLEDSM();
		   j = 0x00;
		}

		SpeakerSM();
		CombineLEDsSM();
		while (!TimerFlag){};
		TimerFlag = 0;
		i = i + time;
		j = j + time;
		k = k + time;
	}
}
