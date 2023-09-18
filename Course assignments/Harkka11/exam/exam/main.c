/*
 * exam.c
 *
 * Created: 3.5.2023 13.42.47
 * Author : jyri_
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>


#define BUZZER_PIN PIND3
#define LED_PIN PINB0
#define ADD_BUTTON_PIN PINB1
#define START_BUTTON_PIN PIND2
#define RESET_BUTTON_PIN PIND3

bool timer_on = false;
int minutes;


void init(){
	
	// Function to initialize the buzzer and LED pins as outputs 
	// and button pins as inputs. Also enables interrupts to call
	// reset_timer interrupt to shut off the buzzer.
}

void counting_time(int minutes){
	
	// Function that delays the program with the given minutes value.
}

reset_timer(INT1_vect){
	
	// Interrupt handler that ends the timing and shuts the buzzer and the LED off
	// when it receives reset signal.
	
	timer_on = false;
	
}

start_timer(INT0_vect){
	
	// Interrupt handler that starts the timer loop by setting the timer_on to true
	// when the start button has been pressed
	
	timer_on = true;
	
}

void buzzer_on (){
	
	// Uses the PWM pin to send proper square waves to the buzzer to make sound.
	
}

int count_minutes(){
	
	// Counts how many times the user has pushed the "Add minute" button.
	
	return minutes
}

void LED_off (){
	// Turns off the LED light to show that the timing is off
}

void LED_on (){
	// Turns on the LED light to show that the timing is on
}

void loop(){
	
	// The main loop of the program
	
	if(timer_on){
		
		// The start button has been clicked, starting the timer
		LED_on();
		
		while(timer_on){
			counting_time(count_minutes());
			buzzer_on();
			// Buzzing stops when reset button is pushed and 
			// timer_on becomes false. 
		}	
		LED_off();
		minutes = 0;
		
	} else{
		// The timer is off, entering sleep mode
		set_sleep_mode(SLEEP_MODE_PWR_DOWN); // set the sleep-mode
		sleep_enable(); // set SE-bit
		sleep_cpu(); // SLEEP-instruction
		sleep_disable(); // reset SE-bit
	}
	
}

int main(void)
{
	init();
    
    while (1) 
    {
		loop();
    }
}

