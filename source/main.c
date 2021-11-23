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
	  threeLEDs = 0x80;
          break;
       case SM_BitTwo:
	  threeLEDs = 0x40;
          break;
       case SM_BitThree:
	  threeLEDs = 0x20;
          break;
       default:
	  break;
    }
}

enum LED_MATRIX_STATES {LED_MATRIX_SMStart, LED_MATRIX_FirstBox, LED_MATRIX_SecondBox, LED_MATRIX_ThirdBox, LED_MATRIX_FourthBox, LED_MATRIX_FifthBox, LED_MATRIX_SixthBox, LED_MATRIX_SeventhBox,
LED_MATRIX_FirstThreeBox, LED_MATRIX_SecondThreeBox, LED_MATRIX_ThirdThreeBox, LED_MATRIX_FourthThreeBox, LED_MATRIX_FifthThreeBox, LED_MATRIX_SixthThreeBox, LED_MATRIX_SeventhThreeBox,
LED_MATRIX_FirstEdgeBox, LED_MATRIX_SecondEdgeBox, LED_MATRIX_ThirdEdgeBox, LED_MATRIX_FourthEdgeBox, LED_MATRIX_FifthEdgeBox, LED_MATRIX_SixthEdgeBox, LED_MATRIX_SeventhEdgeBox,
LED_MATRIX_FirstCenterBox, LED_MATRIX_SecondCenterBox, LED_MATRIX_ThirdCenterBox, LED_MATRIX_FourthCenterBox, LED_MATRIX_FifthCenterBox, LED_MATRIX_SixthCenterBox, LED_MATRIX_SeventhCenterBox,
LED_MATRIX_FirstSingleRightBox, LED_MATRIX_SecondSingleRightBox, LED_MATRIX_ThirdSingleRightBox, LED_MATRIX_FourthSingleRightBox, LED_MATRIX_FifthSingleRightBox, LED_MATRIX_SixthSingleRightBox, LED_MATRIX_SeventhSingleRightBox,
LED_MATRIX_FirstSingleLeftBox, LED_MATRIX_SecondSingleLeftBox, LED_MATRIX_ThirdSingleLeftBox, LED_MATRIX_FourthSingleLeftBox, LED_MATRIX_FifthSingleLeftBox, LED_MATRIX_SixthSingleLeftBox, LED_MATRIX_SeventhSingleLeftBox} LED_MATRIX_STATE;
unsigned char blinkingLED = 0x00;
void LED_MATRIX_SM() {
    switch (LED_MATRIX_STATE) {
       case LED_MATRIX_SMStart:
          LED_MATRIX_STATE = LED_MATRIX_FirstBox;
	  break;
       //start first pattern
       case LED_MATRIX_FirstBox:
	  transmit_data_col(0x66);
	  transmit_data_anti_row(0x3F);
	  LED_MATRIX_STATE = LED_MATRIX_SecondBox;
	  break;
       case LED_MATRIX_SecondBox:
	  transmit_data_col(0x66);
	  transmit_data_anti_row(0x9F);
	  LED_MATRIX_STATE = LED_MATRIX_ThirdBox;
	  break;
       case LED_MATRIX_ThirdBox:
	  transmit_data_col(0x66);
	  transmit_data_anti_row(0xCF);
	  LED_MATRIX_STATE = LED_MATRIX_FourthBox;
	  break;
       case LED_MATRIX_FourthBox:
	  transmit_data_col(0x66);
	  transmit_data_anti_row(0xE7);
	  LED_MATRIX_STATE = LED_MATRIX_FifthBox;
	  break;
       case LED_MATRIX_FifthBox:
	  transmit_data_col(0x66);
	  transmit_data_anti_row(0xF3);
	  LED_MATRIX_STATE = LED_MATRIX_SixthBox;
	  break;
       case LED_MATRIX_SixthBox:
	  transmit_data_col(0x66);
	  transmit_data_anti_row(0xF9);
	  LED_MATRIX_STATE = LED_MATRIX_SeventhBox;
	  break;
       case LED_MATRIX_SeventhBox:
	  transmit_data_col(0x66);
	  transmit_data_anti_row(0xFC);
	  LED_MATRIX_STATE = LED_MATRIX_FirstThreeBox;
	  break;
       //end first pattern
       //start second pattern
       case LED_MATRIX_FirstThreeBox:
	  transmit_data_col(0xDB);
	  transmit_data_anti_row(0x3F);
	  LED_MATRIX_STATE = LED_MATRIX_SecondThreeBox;
	  break;
       case LED_MATRIX_SecondThreeBox:
	  transmit_data_col(0xDB);
	  transmit_data_anti_row(0x9F);
	  LED_MATRIX_STATE = LED_MATRIX_ThirdThreeBox;
	  break;
       case LED_MATRIX_ThirdThreeBox:
	  transmit_data_col(0xDB);
	  transmit_data_anti_row(0xCF);
	  LED_MATRIX_STATE = LED_MATRIX_FourthThreeBox;
	  break;
       case LED_MATRIX_FourthThreeBox:
	  transmit_data_col(0xDB);
	  transmit_data_anti_row(0xE7);
	  LED_MATRIX_STATE = LED_MATRIX_FifthThreeBox;
	  break;
       case LED_MATRIX_FifthThreeBox:
	  transmit_data_col(0xDB);
	  transmit_data_anti_row(0xF3);
	  LED_MATRIX_STATE = LED_MATRIX_SixthThreeBox;
	  break;
       case LED_MATRIX_SixthThreeBox:
	  transmit_data_col(0xDB);
	  transmit_data_anti_row(0xF9);
	  LED_MATRIX_STATE = LED_MATRIX_SeventhThreeBox;
	  break;
       case LED_MATRIX_SeventhThreeBox:
	  transmit_data_col(0xDB);
	  transmit_data_anti_row(0xFC);
	  LED_MATRIX_STATE = LED_MATRIX_FirstEdgeBox;
	  break;
       //end second pattern
       //start third pattern
       case LED_MATRIX_FirstEdgeBox:
	  transmit_data_col(0x7E);
	  transmit_data_anti_row(0x3F);
	  LED_MATRIX_STATE = LED_MATRIX_SecondEdgeBox;
	  break;
       case LED_MATRIX_SecondEdgeBox:
	  transmit_data_col(0x7E);
	  transmit_data_anti_row(0x9F);
	  LED_MATRIX_STATE = LED_MATRIX_ThirdEdgeBox;
	  break;
       case LED_MATRIX_ThirdEdgeBox:
	  transmit_data_col(0x7E);
	  transmit_data_anti_row(0xCF);
	  LED_MATRIX_STATE = LED_MATRIX_FourthEdgeBox;
	  break;
       case LED_MATRIX_FourthEdgeBox:
	  transmit_data_col(0x7E);
	  transmit_data_anti_row(0xE7);
	  LED_MATRIX_STATE = LED_MATRIX_FifthEdgeBox;
	  break;
       case LED_MATRIX_FifthEdgeBox:
	  transmit_data_col(0x7E);
	  transmit_data_anti_row(0xF3);
	  LED_MATRIX_STATE = LED_MATRIX_SixthEdgeBox;
	  break;
       case LED_MATRIX_SixthEdgeBox:
	  transmit_data_col(0x7E);
	  transmit_data_anti_row(0xF9);
	  LED_MATRIX_STATE = LED_MATRIX_SeventhEdgeBox;
	  break;
       case LED_MATRIX_SeventhEdgeBox:
	  transmit_data_col(0x7E);
	  transmit_data_anti_row(0xFC);
	  LED_MATRIX_STATE = LED_MATRIX_FirstCenterBox;
	  break;
       //end third pattern
       //start fourth pattern
       case LED_MATRIX_FirstCenterBox:
	  transmit_data_col(0xE7);
	  transmit_data_anti_row(0x3F);
	  LED_MATRIX_STATE = LED_MATRIX_SecondCenterBox;
	  break;
       case LED_MATRIX_SecondCenterBox:
	  transmit_data_col(0xE7);
	  transmit_data_anti_row(0x9F);
	  LED_MATRIX_STATE = LED_MATRIX_ThirdCenterBox;
	  break;
       case LED_MATRIX_ThirdCenterBox:
	  transmit_data_col(0xE7);
	  transmit_data_anti_row(0xCF);
	  LED_MATRIX_STATE = LED_MATRIX_FourthCenterBox;
	  break;
       case LED_MATRIX_FourthCenterBox:
	  transmit_data_col(0xE7);
	  transmit_data_anti_row(0xE7);
	  LED_MATRIX_STATE = LED_MATRIX_FifthCenterBox;
	  break;
       case LED_MATRIX_FifthCenterBox:
	  transmit_data_col(0xE7);
	  transmit_data_anti_row(0xF3);
	  LED_MATRIX_STATE = LED_MATRIX_SixthCenterBox;
	  break;
       case LED_MATRIX_SixthCenterBox:
	  transmit_data_col(0xE7);
	  transmit_data_anti_row(0xF9);
	  LED_MATRIX_STATE = LED_MATRIX_SeventhCenterBox;
	  break;
       case LED_MATRIX_SeventhCenterBox:
	  transmit_data_col(0xE7);
	  transmit_data_anti_row(0xFC);
	  LED_MATRIX_STATE = LED_MATRIX_FirstSingleRightBox;
	  break;
       //end fifth pattern
       //start fifth pattern
       case LED_MATRIX_FirstSingleRightBox:
	  transmit_data_col(0xFB);
	  transmit_data_anti_row(0x3F);
	  LED_MATRIX_STATE = LED_MATRIX_SecondSingleRightBox;
	  break;
       case LED_MATRIX_SecondSingleRightBox:
	  transmit_data_col(0xFB);
	  transmit_data_anti_row(0x9F);
	  LED_MATRIX_STATE = LED_MATRIX_ThirdSingleRightBox;
	  break;
       case LED_MATRIX_ThirdSingleRightBox:
	  transmit_data_col(0xFB);
	  transmit_data_anti_row(0xCF);
	  LED_MATRIX_STATE = LED_MATRIX_FourthSingleRightBox;
	  break;
       case LED_MATRIX_FourthSingleRightBox:
	  transmit_data_col(0xFB);
	  transmit_data_anti_row(0xE7);
	  LED_MATRIX_STATE = LED_MATRIX_FifthSingleRightBox;
	  break;
       case LED_MATRIX_FifthSingleRightBox:
	  transmit_data_col(0xFB);
	  transmit_data_anti_row(0xF3);
	  LED_MATRIX_STATE = LED_MATRIX_SixthSingleRightBox;
	  break;
       case LED_MATRIX_SixthSingleRightBox:
	  transmit_data_col(0xFB);
	  transmit_data_anti_row(0xF9);
	  LED_MATRIX_STATE = LED_MATRIX_SeventhSingleRightBox;
	  break;
       case LED_MATRIX_SeventhSingleRightBox:
	  transmit_data_col(0xFB);
	  transmit_data_anti_row(0xFC);
	  LED_MATRIX_STATE = LED_MATRIX_FirstSingleLeftBox;
	  break;
       //end fifth pattern
       //start sixth pattern
       case LED_MATRIX_FirstSingleLeftBox:
	  transmit_data_col(0xDF);
	  transmit_data_anti_row(0x3F);
	  LED_MATRIX_STATE = LED_MATRIX_SecondSingleLeftBox;
	  break;
       case LED_MATRIX_SecondSingleLeftBox:
	  transmit_data_col(0xDF);
	  transmit_data_anti_row(0x9F);
	  LED_MATRIX_STATE = LED_MATRIX_ThirdSingleLeftBox;
	  break;
       case LED_MATRIX_ThirdSingleLeftBox:
	  transmit_data_col(0xDF);
	  transmit_data_anti_row(0xCF);
	  LED_MATRIX_STATE = LED_MATRIX_FourthSingleLeftBox;
	  break;
       case LED_MATRIX_FourthSingleLeftBox:
	  transmit_data_col(0xDF);
	  transmit_data_anti_row(0xE7);
	  LED_MATRIX_STATE = LED_MATRIX_FifthSingleLeftBox;
	  break;
       case LED_MATRIX_FifthSingleLeftBox:
	  transmit_data_col(0xDF);
	  transmit_data_anti_row(0xF3);
	  LED_MATRIX_STATE = LED_MATRIX_SixthSingleLeftBox;
	  break;
       case LED_MATRIX_SixthSingleLeftBox:
	  transmit_data_col(0xDF);
	  transmit_data_anti_row(0xF9);
	  LED_MATRIX_STATE = LED_MATRIX_SeventhSingleLeftBox;
	  break;
       case LED_MATRIX_SeventhSingleLeftBox:
	  transmit_data_col(0xDF);
	  transmit_data_anti_row(0xFC);
	  LED_MATRIX_STATE = LED_MATRIX_FirstBox;
	  break;
       //end sixth pattern
       default:
	  LED_MATRIX_STATE = LED_MATRIX_SMStart;
	  break;
    }
    switch (LED_MATRIX_STATE) {
       case LED_MATRIX_SMStart:
          break;
       //first
       case LED_MATRIX_FirstBox:
          break;
       case LED_MATRIX_SecondBox:
          break;
       case LED_MATRIX_ThirdBox:
	  break;
       case LED_MATRIX_FourthBox:
	  break;
       case LED_MATRIX_FifthBox:
	  break;
       case LED_MATRIX_SixthBox:
	  break;
       case LED_MATRIX_SeventhBox:
	  break;
       //second
       case LED_MATRIX_FirstThreeBox:
	  break;
       case LED_MATRIX_SecondThreeBox:
	  break;
       case LED_MATRIX_ThirdThreeBox:
	  break;
       case LED_MATRIX_FourthThreeBox:
	  break;
       case LED_MATRIX_FifthThreeBox:
	  break;
       case LED_MATRIX_SixthThreeBox:
	  break;
       case LED_MATRIX_SeventhThreeBox:
	  break;
       //third
       case LED_MATRIX_FirstEdgeBox:
	  break;
       case LED_MATRIX_SecondEdgeBox:
	  break;
       case LED_MATRIX_ThirdEdgeBox:
	  break;
       case LED_MATRIX_FourthEdgeBox:
	  break;
       case LED_MATRIX_FifthEdgeBox:
	  break;
       case LED_MATRIX_SixthEdgeBox:
	  break;
       case LED_MATRIX_SeventhEdgeBox:
	  break;
       //fourth
       case LED_MATRIX_FirstCenterBox:
	  break;
       case LED_MATRIX_SecondCenterBox:
	  break;
       case LED_MATRIX_ThirdCenterBox:
	  break;
       case LED_MATRIX_FourthCenterBox:
	  break;
       case LED_MATRIX_FifthCenterBox:
	  break;
       case LED_MATRIX_SixthCenterBox:
	  break;
       case LED_MATRIX_SeventhCenterBox:
	  break;
       //fifth
       case LED_MATRIX_FirstSingleRightBox:
	  break;
       case LED_MATRIX_SecondSingleRightBox:
	  break;
       case LED_MATRIX_ThirdSingleRightBox:
	  break;
       case LED_MATRIX_FourthSingleRightBox:
	  break;
       case LED_MATRIX_FifthSingleRightBox:
	  break;
       case LED_MATRIX_SixthSingleRightBox:
	  break;
       case LED_MATRIX_SeventhSingleRightBox:
	  break;
       //sixth
       case LED_MATRIX_FirstSingleLeftBox:
	  break;
       case LED_MATRIX_SecondSingleLeftBox:
	  break;
       case LED_MATRIX_ThirdSingleLeftBox:
	  break;
       case LED_MATRIX_FourthSingleLeftBox:
	  break;
       case LED_MATRIX_FifthSingleLeftBox:
	  break;
       case LED_MATRIX_SixthSingleLeftBox:
	  break;
       case LED_MATRIX_SeventhSingleLeftBox:
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
	  PORTC = (threeLEDs | blinkingLED);
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

void main() {
	
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;

	TimerSet(100);
	TimerOn();

	SM_STATE = SM1_SMStart;
	LED_MATRIX_STATE = LED_MATRIX_SMStart;
	SM3_STATE = SM3_SMStart;

	while(1) {
		ThreeLEDsSM();
		LED_MATRIX_SM();
		CombineLEDsSM();
		while (!TimerFlag);	
		TimerFlag = 0;
	}
}
