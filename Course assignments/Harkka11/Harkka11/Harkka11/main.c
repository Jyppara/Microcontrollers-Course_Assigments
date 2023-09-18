/*
 * Harkka11.c
 *
 * Created: 13.4.2023 10.25.10
 * Author : jyri_
 */ 

#define F_CPU 16000000UL // set the CPU frequency
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>


volatile uint8_t system_active = 1; // initialize the system as active

ISR(PCINT0_vect) // handle the start-button interrupt
{
	if (PINB & (1 << PINB0)) // check if the start-button is pressed
	{
		system_active = 1; // set the system as active
	}
}

int main(void)
{
	DDRB |= (1 << DDB5); // set PB5 as output (LED)
	DDRB &= ~(1 << DDB4); // set PB4 as input (stop-button)
	PORTB |= (1 << PINB4); // enable the pull-up resistor for PB4

	PCICR |= (1 << PCIE0); // enable pin change interrupt 0
	PCMSK0 |= (1 << PCINT0); // enable pin change interrupt for PB0

	sei(); // enable global interrupts

	while (1)
	{
		if (system_active) // check if the system is active
		{
			
			PORTB ^= (1 << PINB5); // toggle the LED state
			_delay_ms(50); 
		}
		else // if the system is inactive
		{
			set_sleep_mode(SLEEP_MODE_PWR_DOWN); // set the sleep mode
			sleep_enable(); // enable sleep mode
			sleep_cpu(); // enter sleep mode
		}

		if (!(PINB & (1 << PINB4))) // check if the stop-button is pressed
		{
			system_active = 0; // set the system as inactive
		}
	}

	return 0;
}

