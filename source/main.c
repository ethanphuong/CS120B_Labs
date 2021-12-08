/*	Your Name & E-mail: Ethan Phuong ephuo001@ucr.edu
 *	Lab Section: 23
 *	Assignment: Final Project
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://youtu.be/cEQCzh8q9PI
 *	Google Drive: https://drive.google.com/drive/folders/1QKCuOdNkEQp_bP-i3GXcuEixsrgoSmk7?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#endif

unsigned char horizontalMovement = 0x00;
unsigned char verticalMovement = 0x00;
unsigned char horizontalMid = 0x8D;
unsigned char verticalMid = 0x97;
unsigned char joystickMove = 0x00;
unsigned char origin = 0x10;
unsigned char difficulty = 0x00;
unsigned char score = 0x00;
unsigned char delay = 0x00;
unsigned char maxDelay = 0x28;
unsigned char prevScore = 0x00;

void InitADC(void)
{
    ADMUX|=(1<<REFS0);    
    ADCSRA|=(1<<ADEN)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2); 
}

enum SCORE_STATES {SCORE_SMStart, SCORE_On} SCORE_STATE;
void SCORE_SM() {
    switch (SCORE_STATE) {
       case SCORE_SMStart:
	  SCORE_STATE = SCORE_On;
	  break;
       case SCORE_On:
	  LCD_ClearScreen();
	  LCD_WriteData(score + '0');
	  LCD_WriteData(prevScore + '0');
	  SCORE_STATE = SCORE_On;
	  break;
       default:
	  SCORE_STATE = SCORE_SMStart;
	  break;
       }
    switch (SCORE_STATE) {
       case SCORE_SMStart:
	  break;
       case SCORE_On:
	  delay++;
	  if (difficulty == 0x01)
	  {
	     maxDelay = 0x32;
	  }
	  else if (difficulty == 0x02)
	  {
	     maxDelay = 0x43;
	  }
	  else if (difficulty == 0x03)
	  {
	     maxDelay = 0x64;
	  }
	  else if (difficulty == 0x04)
	  {
	     maxDelay = 0xC8;
	  }
	  if (delay == maxDelay)
          {
	     delay = 0;
	     if (score == 0x09)
	     {
	     	score = 0;
	     }
	     score++;
	  }
	  break;
       default:
	  break;
    }
}	  

enum MOTION_STATES {MOTION_SMStart, MOTION_On, MOTION_Off} MOTION_STATE;
void MOTION_SM() {
    switch (MOTION_STATE) {
       case MOTION_SMStart:
	  MOTION_STATE = MOTION_On;
	  break;
       case MOTION_On:
	  if ((~PINA & 0x10) == 0x10)
	  {
	     TimerSet(500);
	     difficulty = 0x00;
	  }
	  if ((~PINA & 0x08) == 0x08)
	  {
	     difficulty++;
	     if (difficulty == 0x01)
	     {
		TimerSet(400);
	        MOTION_STATE = MOTION_Off;
	     }
	     else if (difficulty == 0x02)
	     {
		TimerSet(300);
	        MOTION_STATE = MOTION_Off;
	     }
	     else if (difficulty == 0x03)
	     {
		TimerSet(200);
	        MOTION_STATE = MOTION_Off;
	     }
	     else if (difficulty == 0x04)
	     {
		TimerSet(100);
	        MOTION_STATE = MOTION_Off;
	     }
	     else
	     {
		TimerSet(100);
	        MOTION_STATE = MOTION_Off;
	     }
	  }
	  break;
       case MOTION_Off:
	  if ((~PINA & 0x04) == 0x04)
	  {
	     TimerSet(500);
	     difficulty = 0x00;
	     if (score > prevScore) {
	     	prevScore = score;
	     }
	     score = 0x00;
	     delay = 0x00;
	  }
	  if ((~PINA & 0x08) != 0x08)
	  {
	     MOTION_STATE = MOTION_On;
	  }
	  break;
       default:
	  MOTION_STATE = MOTION_SMStart;
	  break;
    }
    switch (MOTION_STATE) {
       case MOTION_SMStart:
	  break;
       case MOTION_On:
	  break;
       case MOTION_Off:
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
void LED_MATRIX_SM() {
    switch (LED_MATRIX_STATE) {
       case LED_MATRIX_SMStart:
	  JOYSTICK();
          LED_MATRIX_STATE = LED_MATRIX_FirstBox;
	  break;
       //start first pattern
       case LED_MATRIX_FirstBox:
	  JOYSTICK();
	  transmit_data_col(0x66);
	  transmit_data_anti_row(0x3F);
	  LED_MATRIX_STATE = LED_MATRIX_SecondBox;
	  break;
       case LED_MATRIX_SecondBox:
	  JOYSTICK();
	  transmit_data_col(0x66);
	  transmit_data_anti_row(0x9F);
	  LED_MATRIX_STATE = LED_MATRIX_ThirdBox;
	  break;
       case LED_MATRIX_ThirdBox:
	  JOYSTICK();
	  transmit_data_col(0x66);
	  transmit_data_anti_row(0xCF);
	  LED_MATRIX_STATE = LED_MATRIX_FourthBox;
	  break;
       case LED_MATRIX_FourthBox:
	  JOYSTICK();
	  transmit_data_col(0x66);
	  transmit_data_anti_row(0xE7);
	  LED_MATRIX_STATE = LED_MATRIX_FifthBox;
	  break;
       case LED_MATRIX_FifthBox:
	  JOYSTICK();
	  transmit_data_col(0x66);
	  transmit_data_anti_row(0xF3);
	  LED_MATRIX_STATE = LED_MATRIX_SixthBox;
	  break;
       case LED_MATRIX_SixthBox:
	  JOYSTICK();
	  transmit_data_col(0x66);
	  transmit_data_anti_row(0xF9);
	  LED_MATRIX_STATE = LED_MATRIX_SeventhBox;
	  break;
       case LED_MATRIX_SeventhBox:
	  JOYSTICK();
	  transmit_data_col(0x66);
	  transmit_data_anti_row(0xFC);
	  LED_MATRIX_STATE = LED_MATRIX_FirstThreeBox;
	  break;
       //end first pattern
       //start second pattern
       case LED_MATRIX_FirstThreeBox:
	  JOYSTICK();
	  transmit_data_col(0xDB);
	  transmit_data_anti_row(0x3F);
	  LED_MATRIX_STATE = LED_MATRIX_SecondThreeBox;
	  break;
       case LED_MATRIX_SecondThreeBox:
	  JOYSTICK();
	  transmit_data_col(0xDB);
	  transmit_data_anti_row(0x9F);
	  LED_MATRIX_STATE = LED_MATRIX_ThirdThreeBox;
	  break;
       case LED_MATRIX_ThirdThreeBox:
	  JOYSTICK();
	  transmit_data_col(0xDB);
	  transmit_data_anti_row(0xCF);
	  LED_MATRIX_STATE = LED_MATRIX_FourthThreeBox;
	  break;
       case LED_MATRIX_FourthThreeBox:
	  JOYSTICK();
	  transmit_data_col(0xDB);
	  transmit_data_anti_row(0xE7);
	  LED_MATRIX_STATE = LED_MATRIX_FifthThreeBox;
	  break;
       case LED_MATRIX_FifthThreeBox:
	  JOYSTICK();
	  transmit_data_col(0xDB);
	  transmit_data_anti_row(0xF3);
	  LED_MATRIX_STATE = LED_MATRIX_SixthThreeBox;
	  break;
       case LED_MATRIX_SixthThreeBox:
	  JOYSTICK();
	  transmit_data_col(0xDB);
	  transmit_data_anti_row(0xF9);
	  LED_MATRIX_STATE = LED_MATRIX_SeventhThreeBox;
	  break;
       case LED_MATRIX_SeventhThreeBox:
	  JOYSTICK();
	  transmit_data_col(0xDB);
	  transmit_data_anti_row(0xFC);
	  LED_MATRIX_STATE = LED_MATRIX_FirstEdgeBox;
	  break;
       //end second pattern
       //start third pattern
       case LED_MATRIX_FirstEdgeBox:
	  JOYSTICK();
	  transmit_data_col(0x7E);
	  transmit_data_anti_row(0x3F);
	  LED_MATRIX_STATE = LED_MATRIX_SecondEdgeBox;
	  break;
       case LED_MATRIX_SecondEdgeBox:
	  JOYSTICK();
	  transmit_data_col(0x7E);
	  transmit_data_anti_row(0x9F);
	  LED_MATRIX_STATE = LED_MATRIX_ThirdEdgeBox;
	  break;
       case LED_MATRIX_ThirdEdgeBox:
	  JOYSTICK();
	  transmit_data_col(0x7E);
	  transmit_data_anti_row(0xCF);
	  LED_MATRIX_STATE = LED_MATRIX_FourthEdgeBox;
	  break;
       case LED_MATRIX_FourthEdgeBox:
	  JOYSTICK();
	  transmit_data_col(0x7E);
	  transmit_data_anti_row(0xE7);
	  LED_MATRIX_STATE = LED_MATRIX_FifthEdgeBox;
	  break;
       case LED_MATRIX_FifthEdgeBox:
	  JOYSTICK();
	  transmit_data_col(0x7E);
	  transmit_data_anti_row(0xF3);
	  LED_MATRIX_STATE = LED_MATRIX_SixthEdgeBox;
	  break;
       case LED_MATRIX_SixthEdgeBox:
	  JOYSTICK();
	  transmit_data_col(0x7E);
	  transmit_data_anti_row(0xF9);
	  LED_MATRIX_STATE = LED_MATRIX_SeventhEdgeBox;
	  break;
       case LED_MATRIX_SeventhEdgeBox:
	  JOYSTICK();
	  transmit_data_col(0x7E);
	  transmit_data_anti_row(0xFC);
	  LED_MATRIX_STATE = LED_MATRIX_FirstCenterBox;
	  break;
       //end third pattern
       //start fourth pattern
       case LED_MATRIX_FirstCenterBox:
	  JOYSTICK();
	  transmit_data_col(0xE7);
	  transmit_data_anti_row(0x3F);
	  LED_MATRIX_STATE = LED_MATRIX_SecondCenterBox;
	  break;
       case LED_MATRIX_SecondCenterBox:
	  JOYSTICK();
	  transmit_data_col(0xE7);
	  transmit_data_anti_row(0x9F);
	  LED_MATRIX_STATE = LED_MATRIX_ThirdCenterBox;
	  break;
       case LED_MATRIX_ThirdCenterBox:
	  JOYSTICK();
	  transmit_data_col(0xE7);
	  transmit_data_anti_row(0xCF);
	  LED_MATRIX_STATE = LED_MATRIX_FourthCenterBox;
	  break;
       case LED_MATRIX_FourthCenterBox:
	  JOYSTICK();
	  transmit_data_col(0xE7);
	  transmit_data_anti_row(0xE7);
	  LED_MATRIX_STATE = LED_MATRIX_FifthCenterBox;
	  break;
       case LED_MATRIX_FifthCenterBox:
	  JOYSTICK();
	  transmit_data_col(0xE7);
	  transmit_data_anti_row(0xF3);
	  LED_MATRIX_STATE = LED_MATRIX_SixthCenterBox;
	  break;
       case LED_MATRIX_SixthCenterBox:
	  JOYSTICK();
	  transmit_data_col(0xE7);
	  transmit_data_anti_row(0xF9);
	  LED_MATRIX_STATE = LED_MATRIX_SeventhCenterBox;
	  break;
       case LED_MATRIX_SeventhCenterBox:
	  JOYSTICK();
	  transmit_data_col(0xE7);
	  transmit_data_anti_row(0xFC);
	  LED_MATRIX_STATE = LED_MATRIX_FirstSingleRightBox;
	  break;
       //end fifth pattern
       //start fifth pattern
       case LED_MATRIX_FirstSingleRightBox:
	  JOYSTICK();
	  transmit_data_col(0xFB);
	  transmit_data_anti_row(0x3F);
	  LED_MATRIX_STATE = LED_MATRIX_SecondSingleRightBox;
	  break;
       case LED_MATRIX_SecondSingleRightBox:
	  JOYSTICK();
	  transmit_data_col(0xFB);
	  transmit_data_anti_row(0x9F);
	  LED_MATRIX_STATE = LED_MATRIX_ThirdSingleRightBox;
	  break;
       case LED_MATRIX_ThirdSingleRightBox:
	  JOYSTICK();
	  transmit_data_col(0xFB);
	  transmit_data_anti_row(0xCF);
	  LED_MATRIX_STATE = LED_MATRIX_FourthSingleRightBox;
	  break;
       case LED_MATRIX_FourthSingleRightBox:
	  JOYSTICK();
	  transmit_data_col(0xFB);
	  transmit_data_anti_row(0xE7);
	  LED_MATRIX_STATE = LED_MATRIX_FifthSingleRightBox;
	  break;
       case LED_MATRIX_FifthSingleRightBox:
	  JOYSTICK();
	  transmit_data_col(0xFB);
	  transmit_data_anti_row(0xF3);
	  LED_MATRIX_STATE = LED_MATRIX_SixthSingleRightBox;
	  break;
       case LED_MATRIX_SixthSingleRightBox:
	  JOYSTICK();
	  transmit_data_col(0xFB);
	  transmit_data_anti_row(0xF9);
	  LED_MATRIX_STATE = LED_MATRIX_SeventhSingleRightBox;
	  break;
       case LED_MATRIX_SeventhSingleRightBox:
	  JOYSTICK();
	  transmit_data_col(0xFB);
	  transmit_data_anti_row(0xFC);
	  LED_MATRIX_STATE = LED_MATRIX_FirstSingleLeftBox;
	  break;
       //end fifth pattern
       //start sixth pattern
       case LED_MATRIX_FirstSingleLeftBox:
	  JOYSTICK();
	  transmit_data_col(0xDF);
	  transmit_data_anti_row(0x3F);
	  LED_MATRIX_STATE = LED_MATRIX_SecondSingleLeftBox;
	  break;
       case LED_MATRIX_SecondSingleLeftBox:
	  JOYSTICK();
	  transmit_data_col(0xDF);
	  transmit_data_anti_row(0x9F);
	  LED_MATRIX_STATE = LED_MATRIX_ThirdSingleLeftBox;
	  break;
       case LED_MATRIX_ThirdSingleLeftBox:
	  JOYSTICK();
	  transmit_data_col(0xDF);
	  transmit_data_anti_row(0xCF);
	  LED_MATRIX_STATE = LED_MATRIX_FourthSingleLeftBox;
	  break;
       case LED_MATRIX_FourthSingleLeftBox:
	  JOYSTICK();
	  transmit_data_col(0xDF);
	  transmit_data_anti_row(0xE7);
	  LED_MATRIX_STATE = LED_MATRIX_FifthSingleLeftBox;
	  break;
       case LED_MATRIX_FifthSingleLeftBox:
	  JOYSTICK();
	  transmit_data_col(0xDF);
	  transmit_data_anti_row(0xF3);
	  LED_MATRIX_STATE = LED_MATRIX_SixthSingleLeftBox;
	  break;
       case LED_MATRIX_SixthSingleLeftBox:
	  JOYSTICK();
	  transmit_data_col(0xDF);
	  transmit_data_anti_row(0xF9);
	  LED_MATRIX_STATE = LED_MATRIX_SeventhSingleLeftBox;
	  break;
       case LED_MATRIX_SeventhSingleLeftBox:
	  JOYSTICK();
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
	  if (!((origin == 0x80) || (origin == 0x10) || (origin == 0x08) || (origin == 0x01)))
	  {
	     TimerSet(500);
	     difficulty = 0x00;
	     if (score > prevScore) {
	        prevScore = score;
	     }
	     score = 0x00;
	     delay = 0x00;
	  }
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
	  if (!((origin == 0x04) || (origin == 0x20)))
	  {
	     TimerSet(500);
	     difficulty = 0x00;
	     if (score > prevScore) {
	        prevScore = score;
	     }
	     score = 0x00;
	     delay = 0x00;
	  }
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
	  if (!((origin == 0x80) || (origin == 0x01)))
	  {
	     TimerSet(500);
	     difficulty = 0x00;
	     if (score > prevScore) {
	        prevScore = score;
	     }
	     score = 0x00;
	     delay = 0x00;
	  }
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
	  if (!((origin == 0x08) || (origin == 0x10)))
	  {
	     TimerSet(500);
	     difficulty = 0x00;
	     if (score > prevScore) {
	        prevScore = score;
	     }
	     score = 0x00;
	     delay = 0x00;
	  }
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
	  if (!(origin == 0x04))
	  {
	     TimerSet(500);
	     difficulty = 0x00;
	     if (score > prevScore) {
	        prevScore = score;
	     }
	     score = 0x00;
	     delay = 0x00;
	  }
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
	  if (!(origin == 0x20))
	  {
	     TimerSet(500);
	     difficulty = 0x00;
	     if (score > prevScore) {
	        prevScore = score;
	     }
	     score = 0x00;
	     delay = 0x00;
	  }
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

void JOYSTICK() {
	switch (ADMUX) {
          case 0x40:
          {
             ADCSRA |=(1<<ADSC);
 	     while ( !(ADCSRA & (1<<ADIF)));
             horizontalMovement = ADC;
             ADC = 0;
             ADMUX = 0x41;
             break;
          }
          case 0x41:
          {
             ADCSRA |=(1<<ADSC);
 	     while ( !(ADCSRA & (1<<ADIF)));
             verticalMovement = ADC;
             ADC = 0;
             ADMUX = 0x40;
             break;
          }
       }
          if (horizontalMovement < (horizontalMid - 50)) {
	     transmit_data_col(origin);
	     transmit_data_anti_row(0xFE);
	     joystickMove |= (1<<PINA3);
	     _delay_ms(5);
	  }
	  if ((~PINA & 0x01) == 0x01) {
	     transmit_data_col(origin / 2);
	     if (origin > 0x01)
	     {
	     	origin = origin / 2;
	     }
	     transmit_data_anti_row(0xFE);
	     joystickMove |= (1<<PINA2);
	     _delay_ms(5);
	  }
	  if (verticalMovement < (verticalMid - 50)) {
	     joystickMove |= (1<<PINA1);
	     _delay_ms(5);
	  }
 	  if ((~PINA & 0x02) == 0x02) {
	     transmit_data_col(origin * 2);
	     if (origin < 0x80)
	     {
	     	origin = origin * 2;
	     }
	     transmit_data_anti_row(0xFE);
	     joystickMove |= (1<<PINA0);
	     _delay_ms(5);
	  }
}

void main() {
	
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

	TimerSet(500);
	TimerOn();
	InitADC();
	LCD_init();

	LED_MATRIX_STATE = LED_MATRIX_SMStart;
	MOTION_STATE = MOTION_SMStart;
	SCORE_STATE = SCORE_SMStart;

	while(1) {
		LED_MATRIX_SM();
		MOTION_SM();
		SCORE_SM();
		while (!TimerFlag);	
		TimerFlag = 0;
	}
}
