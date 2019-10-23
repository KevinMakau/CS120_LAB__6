/*	Author: kmaka003
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
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
	SREG |= 0x08;
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

void TimerSet (unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

typedef enum States {init, LightShow} States;
int LightSM(int);
unsigned char clock = 0;

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0x00; PORTB = 0x00;
    /* Insert your solution below */
	TimerSet(1000);
	TimerOn();
	States state = init;
    while (1) {
	//PORTB = 0xFF;
	state = LightSM(state);
	while (!TimerFlag){	
		TimerFlag = 0;
	}
    }
    return 1;
}

int LightSM (int state){

	switch (state){
		case init:
			state = LightShow;
			break;
		case LightShow:
			state = clock == 3? init: LightShow;
			break;
	}
	switch (state) {
		case init: 
			clock = 0;
			PORTB = 0x00;
			break;
		case LightShow:
			clock += 1;
			if(PORTB == 0x00){
				PORTB = 0x01;
			}
			else {
				PORTB <<= 1;
			}
			break;
	}
	return state;
}











