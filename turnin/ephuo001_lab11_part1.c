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
unsigned char x;
void Tick_LoHi() {
	x = GetKeypadKey();
	switch (SM_STATE) {
		case SM1_SMStart:
		   SM_STATE = SM_Change;
		   break;
		case SM_Change:
		   SM_STATE = SM_Change;
		   break;
		default:
		   SM_STATE = SM1_SMStart;
		   break;
	}
	switch (SM_STATE) {
		case SM1_SMStart:
		   break;
		case SM_Change:
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

int main() {
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;

	static task task1, task2, task3, task4;
	task *tasks[] = { &task1, &task2, &task3, &task4 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	const char start = 0;

	task1.state = start;
	task1.period = 50;
	task1.elapsedTime = task1.period;
	task1.TickFct = &Tick_LoHi;

	TimerSet(50);
	TimerOn();

	unsigned short i;
	while (1) {
		for ( i = 0; i < numTasks; i++) {
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 50;
		}
		while (!TimerFlag);
		TimerFlag = 0;
	}
	return 0;
}
