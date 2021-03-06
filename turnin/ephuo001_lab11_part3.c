/*	Your Name & E-mail: Ethan Phuong ephuo001@ucr.edu
 *	Lab Section: 23
 *	Assignment: Lab 11  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://youtu.be/-DPRc_hKiYE
 */
#include <avr/io.h>
#include <keypad.h>
#include <queue.h>
#include <scheduler.h>
#include <stack.h>
#include <timer.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum SM1_STATES {SM1_SMStart, SM1_Change} SM1_STATE;
unsigned char x;
void Tick_Keypad() {
	x = GetKeypadKey();
	switch (SM1_STATE) {
		case SM1_SMStart:
		   SM1_STATE = SM1_Change;
		   break;
		case SM1_Change:
		   SM1_STATE = SM1_Change;
		   break;
		default:
		   SM1_STATE = SM1_SMStart;
		   break;
	}
	switch (SM1_STATE) {
		case SM1_SMStart:
		   break;
		case SM1_Change:
		   switch(x) {
		      case '\0':
		         PORTB = 0x1F;
		         break;
		      case '0':
		         PORTB = 0x00;
		         break;
		      case '1':
		         PORTB = 0x01;
		         break;
		      case '2':
		         PORTB = 0x02;
		         break;
		      case '3':
		         PORTB = 0x03;
		         break;
		      case '4':
		         PORTB = 0x04;
		         break;
		      case '5':
		         PORTB = 0x05;
		         break;
		      case '6':
		         PORTB = 0x06;
		         break;
		      case '7':
		         PORTB = 0x07;
		         break;
		      case '8':
		         PORTB = 0x08;
		         break;
		      case '9':
		         PORTB = 0x09;
		         break;
		      case 'A':
		         PORTB = 0x0A;
		         break;
		      case 'B':
		         PORTB = 0x0B;
		         break;
		      case 'C':
		         PORTB = 0x0C;
		         break;
		      case 'D':
		         PORTB = 0x0D;
		         break;
		      case '*':
		         PORTB = 0x0E;
		         break;
		      case '#':
		         PORTB = 0x0F;
		         break;
	 	      default:
		         PORTB = 0x1B;
		         break;
		   }
		   break;
		default:
		   break;
	}
}

enum SM2_STATES {SM2_SMStart, SM2_Change} SM2_STATE;
void Tick_LoHi() {
	switch(SM2_STATE)
	{
	   case SM2_SMStart:
	      SM2_STATE = SM2_Change;
	      break;
	   case SM2_Change:
	      SM2_STATE = SM2_Change;
	      break;
	   default:
	      SM2_STATE = SM2_SMStart;
	      break;
	}
	switch(SM2_STATE)
	{
	   case SM2_SMStart:
	      LCD_ClearScreen();
	      break;
	   case SM2_Change:
	      LCD_Cursor(0);
	      LCD_ClearScreen();
	      LCD_WriteData(x);
	      break;
	   default:
	      break;
	}
}

int main() {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;
	DDRD = 0xFF; PORTD = 0x00;

	static task task1, task2;
	task *tasks[] = { &task1, &task2 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	const char start = 0;

	task1.state = start;
	task1.period = 150;
	task1.elapsedTime = task1.period;
	task1.TickFct = &Tick_LoHi;

	task2.state = start;
	task2.period = 10;
	task2.elapsedTime = task2.period;
	task2.TickFct = &Tick_Keypad;

	TimerSet(10);
	TimerOn();

	unsigned short i;
	while (1) {
		for ( i = 0; i < numTasks; i++) {
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 10;
		}
		while (!TimerFlag);
		TimerFlag = 0;
	}
	return 0;
}
