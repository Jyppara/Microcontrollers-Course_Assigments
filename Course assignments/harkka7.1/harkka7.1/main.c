/*
 * harkka7.1.c
 *
 * Created: 21.2.2023 10.56.40
 * Author : jyri_
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define PRESCALER_VALUE 1024
#define OVERFLOWS_FOR_10_YEARS 1205363
#define LED_PIN 5
#define LED_PORT PORTB
volatile uint32_t overflow_count = 0;

ISR(TIMER0_OVF_vect){
	overflow_count++;
}


int main(void)
{
	
	DDRB |= (1 << LED_PIN);		// Setting the pin to output
	TCCR0B |= (1 << CS02) | (1 << CS00);	// setting the prescaler to 1024
	TIMSK0 |= (1 << TOIE0);		// Enabling overflow interrupts
	sei();
	
   while (1)
    {
		if (overflow_count >= OVERFLOWS_FOR_10_YEARS){
			LED_PORT |= (1<< LED_PIN);	// turn on the LED
			while (1){
				// Waits indefinitely
			}
		}
    }
	return 0;
}

