/*
 * Harkka7.c
 *
 * Created: 21.2.2023 9.12.03
 * Author : jyri_
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>

void init_timer(){
	TCCR0B |= (1 << CS00); // No prescaling
	TIMSK0 |= (1 << TOIE0); // Overflow interrupt enable
}

ISR(TIMER0_OVF_vect){
	char forDebug[] = "I'm here!";
}

int main(void)
{
	init_timer();	// Initialize the timer
	
	sei();			// Enable interrupts
	
    while (1) 
    {
		int useless = 0;
    }
}

