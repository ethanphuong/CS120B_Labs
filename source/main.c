/*	Your Name & E-mail: Ethan Phuong ephuo001@ucr.edu
 *	Lab Section: 23
 *	Assignment: Lab 11  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://youtu.be/nxeKS868mJE
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

enum SM_STATES {SM1_SMStart, SM_Change} SM_STATE;
unsigned char x[] = "CS120B is Legend... wait for it Dary!";
unsigned char screen[16];
unsigned char curr = 0x00;
void Tick_LoHi() {
	switch(SM_STATE)
	{
	   case SM1_SMStart:
              if ((~PINA & 0x01) == 0x01)
	      {
	         SM_STATE = SM_Change;
	      }
	      else
	      {
		 SM_STATE = SM1_SMStart;
	      }
	      break;
	   case SM_Change:
	      SM_STATE = SM_Change;
	      break;
	   default:
	      SM_STATE = SM1_SMStart;
	      break;
	}
	switch(SM_STATE)
	{
	   case SM1_SMStart:
	      break;
	   case SM_Change:
	      for (unsigned int i = 0; i < 16; i++)
	      {
	         screen[i] = x[(i + curr) % 37];
	      }
	      curr = ((curr + 1) % 37);
	      LCD_DisplayString(1, screen);
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

	static task task1;
	task *tasks[] = { &task1 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	const char start = 0;

	task1.state = start;
	task1.period = 100;
	task1.elapsedTime = task1.period;
	task1.TickFct = &Tick_LoHi;

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
