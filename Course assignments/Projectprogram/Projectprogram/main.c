/*
 * Projectprogram.c
 *
 * Created: 5.4.2023 8.09.17
 * Author : jyri_
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>


#define F_CPU 16000000UL

#define SWITCH_PIN PINC5 // Pin 5 is connected to the on/off switch (PC5)
#define TEMP_PIN ADC0D // Pin 0 is connected to the temperature sensor (ADC0)

// Define the ADC reference voltage (5V)
#define ADC_REF_VOLTAGE 5.0

// Define the ADC resolution (10 bits)
#define ADC_RESOLUTION 1024.0

// Define the thermistor parameters
#define THERMISTOR_R1 10000.0
#define THERMISTOR_T1 298.15
#define THERMISTOR_B 3975.0

// Define the minimum and maximum angles for the servo motor
#define MIN_ANGLE 0
#define MAX_ANGLE 180

// Define the duty cycle limits for the servo motor
#define MIN_DUTY_CYCLE 1.0
#define MAX_DUTY_CYCLE 2.0

// Define the ADC input pin for the potentiometer
#define POT_PIN ADC0D

// Define the servo motor control pin
#define SERVO_PIN PORTB1

// Constants for the LED
#define LED_PIN PINC3
#define LED_PWM_FREQ 5000
#define LED_MAX_BRIGHTNESS 255
#define LED_BRIGHTNESS_RANGE 100
#define LED_PWM_RANGE (F_CPU / (2 * LED_PWM_FREQ * LED_MAX_BRIGHTNESS))


volatile uint8_t switch_on = 0;
volatile uint16_t servo_pos = 0;
volatile uint16_t temperature = 0;

ISR(INT0_vect)
{
	switch_on = 1; // Set a flag indicating that the switch is on
}

void init_adc()
{
	// Set the ADC reference voltage to AVCC
	ADMUX |= (1 << REFS0);
	
	// Enable the ADC with a prescaler of 128
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1<< ADPS1)| (1 << ADPS0);
}

int read_adc(uint8_t adc_pin)
{
	// Set the ADC input pin
	ADMUX = (ADMUX & 0xF0) | adc_pin;
	
	// Start the ADC conversion
	ADCSRA |= (1<< ADSC);
	
	// Wait for the conversion to finish
	while (ADCSRA & (1 << ADSC));
	
	// Return the ADC result
	
	return ADC;
}

void init_servo()
{
	// Set the servo control pin as an output
	DDRB |= (1 << SERVO_PIN);

	// Set the timer1 clock source to prescaler of 8
	TCCR1B |= (1 << CS11);
	
	// Set the timer1 mode to fast PWM, non inverted output
	TCCR1A |= (1 << COM1A1) | (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13);
	
	// Set the timer1 top value (20ms period)
	ICR1 = 39999;
}

void setup()
{
	// Led pin as output
	DDRB |=(1 << LED_PIN);
	// Configure the switch pin as an input with pull-up resistor
	DDRD &= ~(1 << SWITCH_PIN);
	PORTD |= (1 << SWITCH_PIN);
	
	// Configure external interrupt INT0 on falling edge of the switch pin
	EICRA |= (1<< ISC01);
	EIMSK |= (1 << INT0);
	
	
	// Initialize the UART for sending data to Proteus virtual terminal
	// Set baud rate to 9600
	UBRR0H = 0;
	UBRR0L = 103;
	
	// Enable transmitter and receiver
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);
	
	// Set frame format: 8 data bits, no parity, 1 stop bit
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	
	// Set the initial servo position to the middle angle
	int servo_pos = (MAX_ANGLE - MIN_ANGLE) / 2;
	set_servo_angle(servo_pos);
	
	// Enable global interrupts
	sei();
	
	init_servo();
	init_adc();
	
}

void set_servo_angle(int angle)
{
	// Calculate the duty cycle for the given angle
	double duty_cycle = MIN_DUTY_CYCLE + ((MAX_DUTY_CYCLE - MIN_DUTY_CYCLE)* angle 
	/ (MAX_ANGLE-MIN_ANGLE));
	
	// Calculate the OCR1A value for the given duty cycle
	uint16_t ocr_value = (uint16_t)(duty_cycle * (ICR1 +1)/20.0);
	
	// Set the OCR1A register to the calculated value
	OCR1A = ocr_value;
	
}

double read_temperature()
{
	// Read the ADC value from the thermistor input pin
	int adc_value = read_adc(POT_PIN);
	
	// Convert the ADC value to a resistance value
	double thermistor_r = THERMISTOR_R1 * (ADC_RESOLUTION / adc_value -1.0);
	
	// Convert the resistance value to a temperature value in degrees Celsius
	double thermistor_ln_r = log(thermistor_r);
	double thermistor_t1_ln_r = THERMISTOR_T1 * log(thermistor_r / THERMISTOR_R1);
	double thermistor_t = 1.0 / ((thermistor_ln_r / THERMISTOR_B) + (1.0 / THERMISTOR_T1)) - 273.15;
	
	return thermistor_t;
}

void uart_transmit(char data)
{
	// Wait for empty transmit buffer
	while (!(UCSR0A & (1 << UDRE0)));
	
	// Put data into buffer, sends the data
	UDR0 = data;
}

void uart_puts(char *str)
{
	// Loop through each character in the string
	while (*str != '\0')
	{
		// Transmit the character
		uart_transmit(*str);
		
		// Move to the next character
		str++;
	}
}

void timer1_pwm_init(void)
{
	 TCCR1A |= (1 << COM1A1) | (1 << WGM11);
	 TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11);
	 ICR1 = 255;
}


void loop()
{
	if (switch_on){
		// The switch is on, so operate normally here
		// Read the temperature and set the servo position
		double temperature = read_temperature();
		
		// Read the value of the potentiometer
		int pot_value = read_adc(POT_PIN);
		
		// Convert the potentiometer value to an angle for the servo motor
		int angle = (int)((pot_value/ ADC_RESOLUTION)*(MAX_ANGLE - MIN_ANGLE));
		
		// Set the angle of the servo motor
		set_servo_angle(angle);
		
		// Checks if the virtual terminal has received a '+' or '-' char
		if (UCSR0A & (1 << RXC0)){
			char c = UDR0;
			if (c == '+'){
				angle++;
			}else if (c == '-'){
				angle--;
			}
			set_servo_angle(angle);
		}
		
		// Send the servo position and temperature to Proteus virtual terminal
		char msg[50];
		sprintf(msg, "Servo %u, temperature %u", angle, temperature);
		// uart_puts(msg);
		
		// Set the LED brightness based on the servo position
		OCR1A = (angle * 255) / 180;
		
		// Wait for 2 seconds
		_delay_ms(2000);
		
	}else{
		// The switch is off, entering sleep mode
		set_sleep_mode(SLEEP_MODE_PWR_DOWN); // set the sleep-mode
		sleep_enable(); // set SE-bit
		sleep_cpu(); // SLEEP-instruction
		sleep_disable(); // reset SE-bit
	}
}
int main(void)
{
    setup();
	
    while (1) 
    {
		loop();	
    }
}

