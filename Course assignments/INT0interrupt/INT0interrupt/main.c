/*
 * INT0interrupt.c
 *
 * Created: 31.1.2023 18.22.05
 * Author : jyri_
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>


ISR(INT0_vect){
	
	PORTB = (PINB & 0b00001111) | (PINB << 4);
	
}

int main(void)
{
    
	EIMSK = 0b00000001; // set INT0 to one
	EICRA =  0b000000010; // The rising edge of INT0 generates an interrupt request
	DDRB = 0b11110000; // pins 4-7 as outputs
	PORTB = 0b00000000; // all pins to low
	PCICR = 0b00000100; // enable PCINT23...16 to cause an interrupt
	sei();	// enable global interrupts

	
    while (1) 
    {
		int a = 0;
    }
}